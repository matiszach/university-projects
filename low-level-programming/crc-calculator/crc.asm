
section .bss
    poly resb 8 ; wielomian (po zamianie na liczbe)
    len resb 8 ; dlugosc wielomianu
    fd resb 8 ; deskryptor pliku
    argc resb 8 ; liczba argumentow
    file_name resb 8 ; nazwa pliku
    divisor resb 8 ; wielomian crc
    buffer resb 65536 ; bufor (rozmiar dobrany z uwagi na to, że maksymalna dlugosc fragmentu
                      ; wynosi wlasnie tyle (1 << 16) i nie oplaca sie brac wiekszego poniewaz
                      ; nie wiadomo jakie nastepne bajty trzeba bedzie wczytac z uwagi na dziury w pliku,
                      ; a rownoczesnie nie jest on na tyle duzego rozmiaru by istotnie zwiekszac
                      ; zuzycie pamieci programu, wiec nie ma sensu go pomniejszac bo im wiekszy bufor
                      ; tym mniej wywolan operacji sys_read czyli tym lepiej)
    lookup_table resb 2048 ; tablica pamietajaca wyniki liczenia crc dla kazdego mozliwego bajtu
 
section .text
    global _start
 
_start:


;                      INICJALIZACJA & PREPROCESSING
; [######################################################################]

    ; pobieranie liczby argumentow
    mov rdi, [rsp]
    mov [argc], rdi
    
    ; sprawdzanie czy jest ich dokladnie 3
    cmp qword [argc], 3
    jl .exit
    jg .exit
    
    ; pobieranie nazwy pliku i ciagu binarnego z argumentow
    mov rax, [rsp + 16] ; nazwa pliku
    mov [file_name], rax
    
    mov rax, [rsp + 24] ; ciag binarny
    mov [divisor], rax
    
    ; obliczanie dlugosci ciagu binarnego i konwersja na liczbe
    mov rsi, [divisor] ; zapisanie wskaznika na ciag binarny do rdi
    xor rcx, rcx ; dlugosc ciagu
    xor r8, r8 ; przekonwertowana liczba
.len_loop: ; petla wykonuje sie dopoki bajty sa niezerowe i sprawdza czy wszystkie znaki sa poprawne
    cmp byte [rsi + rcx], 0
    je .len_done
    shl r8, 1
    cmp byte [rsi + rcx], '0'
    je .skip_one
    cmp byte [rsi + rcx], '1'
    jne .exit
    add r8, 1
.skip_one:
    inc rcx
    jmp .len_loop
    
.len_done:
    cmp rcx, 0 ; sprawdzamy czy wielomian nie jest staly
    je .exit
    
    mov [len], rcx ; zapisujemy dlugosc do len
    mov [poly], r8 ; zapisujemy przekonwertowana liczbe do poly
    
    ; przesuwamy wielomian do lewej strony (przesuwajac go o 64 - jego dlugosc bity)
    mov rcx, 64
    sub rcx, [len]
    mov rax, [poly]
    shl rax, cl
    mov [poly], rax
    
    ; obliczanie lookup_table aby moc przetwarzac dane bajt po bajcie
    ; zamiast bit po bicie korzystajac z tego ze dla danego bajtu
    ; zawsze wynik wychodzi taki sam i mozna zapamietac wyniki dla kazdego
    ; bajtu na poczatku programu
    xor r8, r8 ; aktualny bajt
    mov rdi, lookup_table
.byte_loop: ; petla przechodzaca po kazdym mozliwym bajcie
    mov r9, r8 ; r9 to aktualny bajt przesuniety w lewo
    shl r9, 56
    
    ; przejscie sie bit po bicie
    xor rcx, rcx ; aktualny bit bajtu
    dec rcx
.bit_loop:
    inc rcx
    mov r10, r9 ; sprawdzamy czy pierwszy bit r9 to 1
    shr r10, 63
    cmp r10, 1
    je .msb_one
    shl r9, 1 ; jesli pierwszy bit = 0 to r9 = (r9 << 1)
    jmp .end_check
.msb_one: ; jesli pierwszy bit = 1 to r9 = (r9 << 1) ^ poly
    shl r9, 1
    mov r11, [poly]
    xor r9, r11
.end_check:
    cmp rcx, 7
    jl .bit_loop ; koniec petli po bitach
    
    mov qword [rdi + r8 * 8], r9 ; ustawiamy lookup_table od aktualnego bajtu na jego wynik
    
    inc r8
    cmp r8, 256
    jl .byte_loop ; koniec petli po bajtach


;                             OBLICZANIE CRC
; [######################################################################]

    ; otwieranie pliku
    mov rax, 2 ; sys_open
    mov rdi, [file_name] ; nazwa pliku
    xor rsi, rsi ; tryp read-only
    syscall
    test rax, rax ; sprawdzanie czy poprawnie otwarty
    js .exit ; jak nie to blad
    
    ; deskryptor pliku
    mov r8, rax
    
    ; czytanie z pliku
    ; petla bedzie czytala dopoki fragment nie wskaze sam na siebie
    xor r11, r11 ; r11 - tymczasowa zmienna na wczytywane dane
    xor r12, r12 ; r12 - pozycja aktualnie przetwarzanego bajtu
    xor r14, r14 ; r14 - poczatek ostatnio rozpatrywanego fragmentu
    xor r15, r15 ; r15 - wskazuje na aktualna faze czytania (0 - dlugosci, 1 - fragmentu, 2 - przesuniecia)
    xor r13, r13 ; r13 - wynikowa liczba
.read_loop:
    cmp r15, 1
    je .read_data ; r15 = 1 -> czytanie danych
    jg .read_shift ; r15 = 2 -> czytanie przesuniecia
    
    ; r15 = 0 -> czytanie dlugosci
    add r12, 2 ; obecna pozycja += 2
    mov rdx, 2 ; wczytujemy 2 bajty do bufora
    call .read_from_file
    
    ; r11 oznacza tu dlugosc fragmentu
    ; r11 = buffer[0] | (buffer[1] << 8) (little-endian)
    movzx r11, byte [buffer]
    movzx r10, byte [buffer + 1]
    shl r10, 8
    or r11, r10
    mov r15, 1 ; faza -> 1
    
    
    jmp .end_read
.read_data:
    add r12, r11 ; obecna pozycja += dlugosc danych
    mov rdx, r11 ; wczytujemy do bufora r11 bajtow
    mov r10, r11
    call .read_from_file
    
    ; petla liczaca wynik bajt po bajcie
    xor rcx, rcx ; aktualna pozycja
    mov r11, r10
.crc_loop:
    cmp rcx, r11
    jge .end_crc_loop
    
    ; indeks na ktory trzeba spojrzec w lookup_table
    ; idx = (crc ^ (byte << 56)) >> 56
    movzx r9, byte [buffer + rcx]
    shl r9, 56
    mov r10, r13
    xor r10, r9
    shr r10, 56
    ; liczenie wyniku na podstawie wyniku zapisanego dla bajtu
    ; crc = (crc << 8) ^ lookup_table[idx]
    shl r13, 8
    mov rdi, lookup_table
    mov r9, qword [rdi + r10 * 8]
    xor r13, r9
    inc rcx
    jmp .crc_loop
    
.end_crc_loop:
    mov r15, 2 ; faza -> 2
    jmp .end_read

.read_shift:
    add r12, 4 ; obecna pozycja += 4
    mov rdx, 4 ; wczytujemy 4 bajty do bufora
    call .read_from_file
    
    ; r11 oznacza tu przesuniecie
    ; r11 = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24)
    ; (little-endian)
    movzx r11d, byte [buffer]
    movzx r10d, byte [buffer + 1]
    shl r10d, 8
    add r11d, r10d
    movzx r10d, byte [buffer + 2]
    shl r10d, 16
    add r11d, r10d
    movzx r10d, byte [buffer + 3]
    shl r10d, 24
    add r11d, r10d
    
    ; jezeli r11 > MAX_INT to zmieniamy znak
    ; poprzez odjecie 1 i odwrocenie bitow
    mov r9, 0x7fffffff
    cmp r11, r9
    jle .not_greater
    ; przesuniecie jest ujemne
    sub r11d, 1
    not r11d
    sub r12, r11 ; obecna pozycja -= przesuniecie
    jmp .greater
.not_greater:
	; przesuniecie jest dodatnie
    add r12, r11 ; obecna pozycja += przesuniecie
.greater:
    ; przesuwanie aktualnie czytanego bajtu o przesuniecie (lseek)
    mov rdi, r8 ; deskryptor pliku
    mov rsi, r12 ; przesuwamy o przesuniecie
    mov rdx, 0 ; SEEK_SET (wzgledem aktualnej pozycji)
    mov rax, 8 ; lseek
    syscall
    cmp rax, 0 ; sprawdzanie czy sie udalo
    jl .exit
    
    cmp r12, r14 ; jezeli obecna pozycja jest na poczatku tego aktualnego fragmentu to konczymy petle
    je .break_read_loop
    mov r14, r12 ; ostatnia pozycja -> aktualna pozycja
    mov r15, 0 ; faza -> 0
.end_read:
    jmp .read_loop
    
.break_read_loop: ; wyjscie z petli

;                            WYPISYWANIE DANYCH
; [######################################################################]

    ; zamykanie pliku
    mov rax, 3 ; sys_close
    mov rdi, [fd] ; deskryptor pliku
    syscall
    test rax, rax ; sprawdzanie czy sie udalo
    js .exit
    
    ; wypisywanie wyniku bit po bicie
    mov r9, [len] ; ustawienie r9 na dlugosc wielomianu wejsciowego
.write_loop:
    mov r12, r13 ; sprawdzenie czy pierwszy bit = 1
    mov rcx, 63
    shr r12, cl
    cmp r12, 1
    je .is_one
    mov byte [buffer], '0' ; wrzucanie odpowiednio '0' i '1' do bufora aby je wypisywac
    jmp .skip
.is_one:
    mov byte [buffer], '1'
.skip:

    ; wypisywanie bitu (bajtu)
    mov rsi, buffer
    mov rdx, 1
    call .print_data
    
    shl r13, 1 ; przesuwanie r13 w lewo aby kolejny bit byl na pierwszej pozycji
    dec r9
    cmp r9, 0
    jg .write_loop ; koniec petli
    
    ; wypisanie znaku nowego wiersza
    mov byte [buffer], 10
    mov rsi, buffer
    mov rdx, 1
    call .print_data
    
    ; zakonczenie programu
    mov rdi, 0
    mov rax, 60
    syscall


;                                 FUNCKJE
; [######################################################################]

; program zakonczony bledem
.exit:
    mov rdi, 1
    mov rax, 60
    syscall


; czytanie z pliku do bufora
.read_from_file:
    mov rdi, r8
    mov rsi, buffer
    mov rax, 0
    syscall
    test rax, rax
    js .exit
    ret


; wypisywanie danych
.print_data:
    mov rdi, 1
    mov rax, 1
    syscall
    test rax, rax
    js .exit
    ret

