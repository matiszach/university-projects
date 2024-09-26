# Bignum Division Function

## Warning

The Project uses system-specific functionalities and works only on `Linux`.

## Description

Assembly implementation of C function for calculating quotient and remainder of bignum division.
```c
int64_t mdiv(int64_t *x, size_t n, int64_t y);
```

The function performs integer division with remainder. It treats the dividend, divisor, quotient, and remainder as numbers encoded in two's complement form. The first and second parameters define the dividend: `x` is a pointer to a non-empty array of `n` 64-bit integers. The dividend consists of `64 * n` bits and is stored in memory using little-endian order. The third parameter, `y`, is the divisor. The function returns the remainder of dividing `x` by `y`. Additionally, the quotient is stored in the array `x`.

If the quotient cannot be stored in the array `x`, this indicates an overflow. The function handles overflow in the same way as the `div` and `idiv` instructions, raising an interrupt with number 0.

## Usage

Compile the program.
```
nasm -f elf64 -w+all -w+error -o mdiv.o mdiv.asm
```

Then compile and consolidate it with your `C` file.
```
gcc -c -Wall -Wextra -std=c17 -O2 -o your_program.o your_program.c
gcc -z noexecstack -o your_program your_program.o mdiv.o
```
