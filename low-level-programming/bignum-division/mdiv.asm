global mdiv

section .text

mdiv:
    ; rdi - (x)
    ; rsi - (n)
    ; rdx - (y)

    ; rbx - divisor
    mov rbx, rdx

    ; r11 - array size
    mov r11, rsi

    ; edge case
    ; x = INT_MIN and y = -1
    cmp rbx, -1 ; check if y = -1
    jne not_edge_case
    mov r8, 0x8000000000000000 ; set r8 to INT_MIN for comparison
    cmp qword [rdi + rsi*8 - 8], r8 ; check if x[n - 1] = INT_MIN
    jne not_edge_case
    dec rsi

    ; loop to check if x[0], ..., x[n - 2] = 0
check_zero_loop: 
    cmp rsi, 0 ; if rsi = 0, exit loop
    je end_check_zero_loop
    cmp qword [rdi + rsi*8 - 8], 0 ; if x[i] != 0, x != INT_MIN 
    jne not_edge_case

    dec rsi
    jmp check_zero_loop

end_check_zero_loop:
    ; if the edge case holds, return SIGFPE
    ; by forcing division by 0
    xor rax, rax
    div rax

not_edge_case: ; if not an edge case, continue

    ; set r9 and r10 to boolean values indicating if x < 0 and if y < 0

    ; r9 - is y less than 0
    xor r9, r9
    cmp rbx, 0
    jge y_not_negative ; if y < 0, set r9 = 1 and set y = -y
    mov r9, 1
    neg rbx
y_not_negative: ; end check if y < 0

    ; r10 - is x less than 0
    xor r10, r10
    cmp qword [rdi + rsi*8 - 8], 0
    jge x_not_negative ; if x < 0, set r10 = 1 and set x = -x
    mov r10, 1

    ; negate x
    ; by subtracting 1 and negating the bits

    ; subtracting 1 from x manually
    xor rsi, rsi
    mov r8, 0xffffffffffffffff ; set r8 to INT_MAX

decrement_loop:
    dec qword [rdi + rsi*8] ; decrement x[i]
    cmp qword [rdi + rsi*8], r8
    jne stop_decrementing ; check if x[i] != INT_MAX, if so, stop subtracting

    inc rsi
    cmp rsi, r11
    jl decrement_loop

stop_decrementing:

    ; negate the bits of x
    xor rsi, rsi

negate_bits_loop:
    not qword [rdi + rsi*8] ; negate the bits of x

    inc rsi
    cmp rsi, r11
    jl negate_bits_loop

x_not_negative: ; end check if x < 0

    ; manual division of x

    mov rsi, r11 ; loop from i = n - 1
    xor rdx, rdx ; clear remainder

division_loop:
    mov rax, qword [rdi + rsi*8 - 8] ; load x[i] into rax

    div rbx ; divide (rdx << 64 + rax) by rbx
    mov [rdi + rsi*8 - 8], rax ; set x[i] to division result

    dec rsi
    cmp rsi, 0
    jg division_loop

    mov rax, rdx ; set function result (mdiv) to the remainder of the division

    ; check the signs of the result x and remainder

    ; if exactly one of (x < 0) and (y < 0) is true, x should be negative
    xor r9, r10 
    cmp r9, 0
    je x_not_negative_result ; if r9 = 1, negate x

    ; negate x
    ; by negating bits and adding 1

    ; negate the bits of x
    xor rsi, rsi ; loop from 0

negate_bits_result_loop:
    not qword [rdi + rsi*8] ; negate the bits of x

    inc rsi
    cmp rsi, r11
    jl negate_bits_result_loop

    ; manually add 1 to x
    xor rsi, rsi ; loop from 0

increment_loop:
    inc qword [rdi + rsi*8] ; increment x[i]
    cmp qword [rdi + rsi*8], 0
    jne x_not_negative_result ; if x[i] != 0, stop adding

    inc rsi
    cmp rsi, r11
    jl increment_loop

x_not_negative_result: ; end check if x should be negative

    ; if x < 0, the remainder is negative
    cmp r10, 0
    je remainder_not_negative ; check if x < 0, if so, negate the remainder
    neg rax

remainder_not_negative:

    ret ; end of function
