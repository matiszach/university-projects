# CRC calculator

## Description

This project is an assembly program which calculates the [CRC](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) checksum for the given input file and CRC polynomial.

This program computes the CRC checksum of a Linux file, which may contain sparse regions ("holes"). A sparse file consists of continuous segments, where each segment begins with a 2-byte length field, followed by the data. The segment ends with a 4-byte offset indicating how far to move to reach the next segment. The length is a 16-bit unsigned integer in little-endian format, and the offset is a 32-bit two's complement number. The first segment starts at the file's beginning, and the last segment is identified when its offset points to itself.

The program processes each byte in these segments using the provided CRC polynomial to calculate the file's checksum, with the most significant bits of both the data and the CRC polynomial aligned to the left.
