# CRC calculator

## Warning

The Project uses system-specific functionalities and works only on `Linux`.

## Description

This project is an assembly program which calculates the [CRC](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) checksum for the given input file and CRC polynomial.

The program calculates the CRC checksum of a sparse Linux file. Sparse files are files that can contain "holes" (empty regions) and are organized into continuous segments.

Each segment in the file consists of:
- **2-byte length**: The length of the data in the segment.
- **Data**: The actual file data for the segment.
- **4-byte offset**: Indicates how many bytes to skip from the end of the current segment to the start of the next.

### Important Details:
- The length field is a 16-bit unsigned integer in little-endian format.
- The offset is a 32-bit signed integer (two's complement) in little-endian format.
- The first segment starts at the file's beginning.
- The last segment is identified when its offset points to itself.
- Segments may touch or overlap.

### CRC Checksum Calculation:
The checksum is calculated using the **Cyclic Redundancy Check (CRC)** algorithm. The data in each segment is processed byte-by-byte. Both the file's data and the user-specified CRC polynomial are treated with their most significant bits (MSBs) aligned to the left, following the standard approach for CRC calculations.

### Optimization

The program utilizes a buffer for more efficient data processing.

An array with an answer for every possible byte is preprocessed at the start allowing it to process the data byte by byte instead of bit by bit.

## Usage

Compile the program.
```
nasm -f elf64 -w+all -w+error -o crc.o crc.asm
ld --fatal-warnings -o crc crc.o
```

Run the program. The program takes 2 arguments; the input file's name and the CRC polynomial.
```
./crc file crc_poly
```

- **file**: The name of the file to be processed.
- **crc_poly**: A binary string representing the CRC polynomial. The highest-order coefficient is omitted, and the maximum degree of the CRC polynomial is 64 (resulting in a maximum CRC divisor length of 65 bits). 
  - For example, `11010101` represents the polynomial: 𝑥⁸ + 𝑥⁷ + 𝑥⁶ + 𝑥⁴ + 𝑥² + 1.
  - Constant polynomials are considered invalid.
