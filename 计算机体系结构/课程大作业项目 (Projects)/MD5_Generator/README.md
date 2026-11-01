# MD5-Generator

## Overview
This project implements the MD5 cryptographic hash algorithm using the RISC-V instruction set architecture. MD5 produces a 128-bit hash value and is commonly used for data integrity verification.

## Features
- Complete implementation of the MD5 algorithm in RISC-V assembly
- Input handling for various message sizes

## Implementation Details
The implementation follows the standard MD5 algorithm which includes:
- Padding the message
- Processing message in 512-bit blocks
- Four rounds of operations with specific functions
- Output of a 128-bit digest

## Testing
Includes test cases to verify correct implementation against known MD5 hashes.
```
java -jar venus-jvm-latest.jar -cc main.S > md5.out
```