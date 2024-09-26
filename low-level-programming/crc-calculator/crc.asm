
section .bss
    poly resb 8 ; polynomial (after conversion to an integer)
    len resb 8 ; polynomial length
    fd resb 8 ; file descriptor
    argc resb 8 ; number of arguemnts
    file_name resb 8 ; file name
    divisor resb 8 ; CRC polynomial
    buffer resb 65536 ; buffer (size chosen because the maximum fragment length
                      ; is exactly that (1 << 16) and it is not worth taking a larger one because
                      ; it is not known what next bytes will have to be loaded due to holes in the file,
                      ; and at the same time it is not large enough to significantly increase
                      ; program memory usage, so there is no point in reducing it because the larger the buffer
                      ; the fewer sys_read operation calls, i.e. the better)
    lookup_table resb 2048 ; array storing the results of the crc calculation for each possible byte
 
section .text
    global _start
 
_start:


;                      INITIALIZATION & PREPROCESSING
; [######################################################################]

    ; getting the arguments
    mov rdi, [rsp]
    mov [argc], rdi
    
    ; checking if there are exactly 3 of them
    cmp qword [argc], 3
    jl .exit
    jg .exit
    
    ; getting file name and binary sequence from arguments
    mov rax, [rsp + 16] ; file name
    mov [file_name], rax
    
    mov rax, [rsp + 24] ; binary string
    mov [divisor], rax
    
    ; calculating the length of the binary string and converting it to a number
    mov rsi, [divisor] ; store the pointer to the binary string in rsi
    xor rcx, rcx ; string length
    xor r8, r8 ; converted number
.len_loop: ; loop continues as long as bytes are non-zero and checks if all characters are valid
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
    cmp rcx, 0 ; check if the polynomial is constant
    je .exit
    
    mov [len], rcx ; store the length in len
    mov [poly], r8 ; store the converted number in poly
    
    ; shift the polynomial to the left (shifting it by 64 - its length bits)
    mov rcx, 64
    sub rcx, [len]
    mov rax, [poly]
    shl rax, cl
    mov [poly], rax
    
    ; calculating the lookup table to process data byte by byte
    ; instead of bit by bit by leveraging the fact that for a given byte
    ; the result is always the same, and we can store results for each
    ; byte at the start of the program
    xor r8, r8 ; current byte
    mov rdi, lookup_table
.byte_loop: ; loop iterating through every possible byte
    mov r9, r8 ; r9 is the current byte shifted left
    shl r9, 56
    
    ; iterating through each bit
    xor rcx, rcx ; current bit of the byte
    dec rcx
.bit_loop:
    inc rcx
    mov r10, r9 ; check if the first bit of r9 is 1
    shr r10, 63
    cmp r10, 1
    je .msb_one
    shl r9, 1 ; if the first bit = 0 then r9 = (r9 << 1)
    jmp .end_check
.msb_one: ; if the first bit = 1 then r9 = (r9 << 1) ^ poly
    shl r9, 1
    mov r11, [poly]
    xor r9, r11
.end_check:
    cmp rcx, 7
    jl .bit_loop ; end of the bit loop
    
    mov qword [rdi + r8 * 8], r9 ; set lookup_table for the current byte with its result
    
    inc r8
    cmp r8, 256
    jl .byte_loop ; end of the byte loop


;                             CRC CALCULATION
; [######################################################################]

    ; opening the file
    mov rax, 2 ; sys_open
    mov rdi, [file_name] ; file name
    xor rsi, rsi ; read-only mode
    syscall
    test rax, rax ; check if successfully opened
    js .exit ; if not, exit with error
    
    ; file descriptor
    mov r8, rax
    
    ; reading from the file
    ; the loop reads until the fragment points to itself
    xor r11, r11 ; r11 - temporary variable for the data being read
    xor r12, r12 ; r12 - position of the currently processed byte
    xor r14, r14 ; r14 - start of the last considered fragment
    xor r15, r15 ; r15 - indicates the current phase of reading (0 - length, 1 - fragment, 2 - shift)
    xor r13, r13 ; r13 - resulting number
.read_loop:
    cmp r15, 1
    je .read_data ; r15 = 1 -> reading data
    jg .read_shift ; r15 = 2 -> reading shift
    
    ; r15 = 0 -> reading length
    add r12, 2 ; current position += 2
    mov rdx, 2 ; read 2 bytes into the buffer
    call .read_from_file
    
    ; r11 here means the length of the fragment
    ; r11 = buffer[0] | (buffer[1] << 8) (little-endian)
    movzx r11, byte [buffer]
    movzx r10, byte [buffer + 1]
    shl r10, 8
    or r11, r10
    mov r15, 1 ; phase -> 1
    
    jmp .end_read
.read_data:
    add r12, r11 ; current position += data length
    mov rdx, r11 ; read r11 bytes into the buffer
    mov r10, r11
    call .read_from_file
    
    ; loop processing the result byte by byte
    xor rcx, rcx ; current position
    mov r11, r10
.crc_loop:
    cmp rcx, r11
    jge .end_crc_loop
    
    ; index to check in lookup_table
    ; idx = (crc ^ (byte << 56)) >> 56
    movzx r9, byte [buffer + rcx]
    shl r9, 56
    mov r10, r13
    xor r10, r9
    shr r10, 56
    ; calculating the result based on the stored result for the byte
    ; crc = (crc << 8) ^ lookup_table[idx]
    shl r13, 8
    mov rdi, lookup_table
    mov r9, qword [rdi + r10 * 8]
    xor r13, r9
    inc rcx
    jmp .crc_loop
    
.end_crc_loop:
    mov r15, 2 ; phase -> 2
    jmp .end_read

.read_shift:
    add r12, 4 ; current position += 4
    mov rdx, 4 ; read 4 bytes into the buffer
    call .read_from_file
    
    ; r11 here means the shift
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
    
    ; if r11 > MAX_INT then change the sign
    ; by subtracting 1 and inverting the bits
    mov r9, 0x7fffffff
    cmp r11, r9
    jle .not_greater
    ; the shift is negative
    sub r11d, 1
    not r11d
    sub r12, r11 ; current position -= shift
    jmp .greater
.not_greater:
	; the shift is positive
    add r12, r11 ; current position += shift
.greater:
    ; shift the currently read byte by the shift amount (lseek)
    mov rdi, r8 ; file descriptor
    mov rsi, r12 ; apply shift
    mov rdx, 0 ; SEEK_SET (relative to current position)
    mov rax, 8 ; lseek
    syscall
    cmp rax, 0 ; check if successful
    jl .exit
    
    cmp r12, r14 ; if current position is at the start of this fragment, end the loop
    je .break_read_loop
    mov r14, r12 ; last position -> current position
    mov r15, 0 ; phase -> 0
.end_read:
    jmp .read_loop
    
.break_read_loop: ; exit the loop

;                            OUTPUTTING DATA
; [######################################################################]

    ; closing the file
    mov rax, 3 ; sys_close
    mov rdi, [fd] ; file descriptor
    syscall
    test rax, rax ; check if successful
    js .exit
    
    ; outputting the result bit by bit
    mov r9, [len] ; set r9 to the length of the input polynomial
.write_loop:
    mov r12, r13 ; check if the first bit = 1
    mov rcx, 63
    shr r12, cl
    cmp r12, 1
    je .is_one
    mov byte [buffer], '0' ; insert '0' or '1' into the buffer for output
    jmp .skip
.is_one:
    mov byte [buffer], '1'
.skip:

    ; outputting a bit (byte)
    mov rsi, buffer
    mov rdx, 1
    call .print_data
    
    shl r13, 1 ; shift r13 left so the next bit is in the first position
    dec r9
    cmp r9, 0
    jg .write_loop ; end of loop
    
    ; outputting a newline character
    mov byte [buffer], 10
    mov rsi, buffer
    mov rdx, 1
    call .print_data
    
    ; program termination
    mov rdi, 0
    mov rax, 60
    syscall


;                                 FUNCTIONS
; [######################################################################]

; program exited with an error
.exit:
    mov rdi, 1
    mov rax, 60
    syscall


; reading from the file into the buffer
.read_from_file:
    mov rdi, r8
    mov rsi, buffer
    mov rax, 0
    syscall
    test rax, rax
    js .exit
    ret


; outputting data
.print_data:
    mov rdi, 1
    mov rax, 1
    syscall
    test rax, rax
    js .exit
    ret
