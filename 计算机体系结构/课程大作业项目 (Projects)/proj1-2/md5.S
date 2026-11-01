
  .data

# Initial buffer value
initial_buffer:
  .word 0x67452301 # A
  .word 0xefcdab89 # B
  .word 0x98badcfe # C
  .word 0x10325476 # D

# 64-element table constructed from the sine function.
K:
  .word 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee
  .word 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501
  .word 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be
  .word 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821
  .word 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa
  .word 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8
  .word 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed
  .word 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a
  .word 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c
  .word 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70
  .word 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05
  .word 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665
  .word 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039
  .word 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1
  .word 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1
  .word 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391

# Per-round shift amounts
S:
  .word 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22
  .word 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20
  .word 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23
  .word 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21



  .text

# -----------------------------------------------------------------
# Function: md5
#
# Description:
#   Computes the MD5 hash of an input message using the MD5 algorithm.
#   The computed digest is stored in memory.
#
# Parameters:
#   a0 - Pointer to the beginning of the input message.
#   a1 - Length of the input message in bytes (original length).
#   a2 - Pointer to a memory region where the resulting MD5 digest
#        will be stored.
# -----------------------------------------------------------------
  .globl md5
md5:
  addi sp, sp, -128
  sw ra, 124(sp)
  sw s0, 120(sp)
  sw s1, 116(sp)
  sw s2, 112(sp)
  sw s3, 108(sp)
  sw s4, 104(sp)
  sw s5, 100(sp)
  sw s6, 96(sp)
  sw s7, 92(sp)
  sw s8, 88(sp)
  sw s9, 84(sp)
  sw s10, 80(sp)
  sw s11, 76(sp)

  # s0 = Pointer to the 64-byte buffer on the stack
  addi s0, sp, 0          # Buffer starts at the bottom of the allocated frame

  mv s1, a0               # s1 = current input pointer
  mv s2, a1               # s2 = remaining length of original message
  mv s3, a2               # s3 = result pointer (where to store final hash)
  mv s10, a1              # s10 = original length (needed for padding)

  # Initialize hash state variables (A, B, C, D)
  la t0, initial_buffer
  lw s6, 0(t0)
  lw s7, 4(t0)
  lw s8, 8(t0)
  lw s9, 12(t0)

process_next_chunk:
  # Check if remaining length is >= 64 bytes
  li t0, 64
  blt s2, t0, process_last_block

  # Process a full 64-byte block directly from input
  mv a0, s1               # a0 = pointer to current block in input
  jal process_single_block # Process the block, updates s6-s9

  addi s1, s1, 64         # Move input pointer to next block
  sub s2, s2, t0          # Decrement remaining length
  j process_next_chunk

process_last_block:
  # Handle the last part of the message (less than 64 bytes)
  # Copy remaining data to the stack buffer (s0)
  mv t0, s0               # t0 = destination (stack buffer)
  mv t1, s1               # t1 = source (remaining input)
  mv t2, s2               # t2 = count (remaining length)
copy_last_bytes:
  beqz t2, pad_start
  lb t3, 0(t1)
  sb t3, 0(t0)
  addi t0, t0, 1
  addi t1, t1, 1
  addi t2, t2, -1
  j copy_last_bytes

pad_start:
  # t0 now points to the byte after the copied message data in the buffer
  # s2 holds the original number of bytes copied (original remaining length)

  # Append the '1' bit (0x80)
  li t3, 0x80
  sb t3, 0(t0)
  addi t0, t0, 1          # Increment buffer pointer

  # Calculate current position within the 64-byte block
  sub t4, t0, s0          # t4 = number of bytes used so far (message + 0x80)

  # Zero padding loop: Pad until byte 56
pad_zeros:
  li t1, 56
  beq t4, t1, append_length # If we are at byte 56, append length
  sb zero, 0(t0)          # Append a zero byte
  addi t0, t0, 1
  addi t4, t4, 1
  li t1, 64
  beq t4, t1, process_and_reset # If buffer is full, process and reset
  j pad_zeros             # Continue padding zeros

process_and_reset:
  # Buffer is full, process this block
  mv a0, s0               # a0 = pointer to the filled buffer
  jal process_single_block # Process the block

  # Reset buffer pointer and byte count for the next (length-only) block
  mv t0, s0               # t0 points to start of buffer
  li t4, 0                # Reset byte count
  j pad_zeros             # Continue padding zeros in the new block

append_length:
  # We are at byte 56 (t4 = 56). Append original length in bits.
  slli t5, s10, 3         # t5 = original length in bits (lower 32 bits)
  sw t5, 0(t0)            # Store lower 32 bits of length
  addi t0, t0, 4
  sw zero, 0(t0)          # Store upper 32 bits of length (as 0)

  # Process this final padded block
  mv a0, s0               # a0 = pointer to the buffer containing padding + length
  jal process_single_block # Process the block

  j md5_done              # Finished processing all blocks

# -----------------------------------------------------------------
# Internal Helper: process_single_block
# Description: Processes one 64-byte message block.
# Parameters:
#   a0 - Pointer to the 64-byte block M
#   s6, s7, s8, s9 - Current hash state (A, B, C, D) - Modified by function
# Uses: s4-s11, t0-t6 (Modifies s6-s9)
# Clobbers: t0-t6
# Preserves: s0-s3, s10 (relative to caller)
# -----------------------------------------------------------------
process_single_block:
  # Allocate space on stack for B_init, C_init, D_init (12 bytes)
  addi sp, sp, -12
  sw s7, 8(sp)            # Save B_init on stack
  sw s8, 4(sp)            # Save C_init on stack
  sw s9, 0(sp)            # Save D_init on stack

  # Save initial hash values for this block
  mv s4, s6               # s4 = A_init
  # s5 is now free to be used as g index register
  # s10 holds original message length - DO NOT MODIFY
  # B_init, C_init, D_init are now on the stack

  # Main loop: 64 rounds
  li s11, 0               # i = 0
process_operations:         # Label retained for round jumps
  # Determine which round function and g value to use based on i (s11)
  li t0, 16
  blt s11, t0, round1_calc
  li t0, 32
  blt s11, t0, round2_calc
  li t0, 48
  blt s11, t0, round3_calc
  j round4_calc

round1_calc:
  # F = (B & C) | (~B & D)
  and t3, s7, s8
  not t4, s7
  and t4, t4, s9
  or t3, t3, t4
  # g = i
  mv s5, s11
  j update_all
round2_calc:
  # F = (D & B) | (~D & C)
  and t3, s7, s9
  not t4, s9
  and t4, t4, s8
  or t3, t3, t4
  # g = (5*i + 1) % 16
  li t1, 5
  mul s5, s11, t1
  addi s5, s5, 1
  li t1, 16
  rem s5, s5, t1
  j update_all
round3_calc:
  # F = B ^ C ^ D
  xor t3, s7, s8
  xor t3, t3, s9
  # g = (3*i + 5) % 16
  li t1, 3
  mul s5, s11, t1
  addi s5, s5, 5
  li t1, 16
  rem s5, s5, t1
  j update_all
round4_calc:
  # F = C ^ (B | ~D)
  not t3, s9
  or t3, s7, t3
  xor t3, t3, s8
  # g = (7*i) % 16
  li t1, 7
  mul s5, s11, t1
  li t1, 16
  rem s5, s5, t1
  # Fall through to update_all

update_all:
  # F is in t3, g is in s5
  # Calculate address of M[g] from the block pointer in a0
  slli t0, s5, 2          # t0 = g * 4
  add t0, t0, a0          # t0 = a0 + g * 4 = address of M[g]
  lw t0, 0(t0)            # t0 = M[g]

  # Load K[i]
  la t2, K
  mv t1, s11              # t1 = i
  slli t1, t1, 2          # t1 = i * 4
  add t2, t2, t1          # t2 = K + i * 4 = address of K[i]
  lw t1, 0(t2)            # t1 = K[i]

  # Temp = F + A + K[i] + M[g]
  # F is in t3, A is in s6, K[i] is in t1, M[g] is in t0
  add t3, t3, s6          # Temp = F + A
  add t3, t3, t1          # Temp = F + A + K[i]
  add t3, t3, t0          # Temp = F + A + K[i] + M[g]

  # Load shift amount s[i]
  la t2, S
  slli t4, s11, 2         # t4 = i * 4
  add t2, t2, t4          # t2 = S + i * 4 = address of S[i]
  lw t4, 0(t2)            # t4 = s[i] (shift amount)

  # Left rotate Temp by s[i] bits
  sll t5, t3, t4          # Temp << s[i]
  li t1, 32
  sub t1, t1, t4          # 32 - s[i]
  srl t2, t3, t1          # Temp >> (32 - s[i])
  or t4, t5, t2           # t4 = leftrotate(Temp, s[i])

  # New hash values
  # A = D
  # D = C
  # C = B
  # B = B + leftrotate(Temp, s[i])
  mv t1, s7               # Save B for C calculation
  add s7, s7, t4          # New B = B + leftrotate(Temp, s[i])
  mv s6, s9               # New A = D
  mv s9, s8               # New D = C
  mv s8, t1               # New C = old B

  # Increment round counter and loop
  addi s11, s11, 1
  li t0, 64
  blt s11, t0, process_operations # Loop if i < 64

  # End of 64 rounds for this block
  # Load initial B, C, D from stack
  lw t4, 8(sp)            # t4 = B_init (using t4 temporarily)
  lw t5, 4(sp)            # t5 = C_init
  lw t6, 0(sp)            # t6 = D_init

  # Add initial hash values back
  add s6, s6, s4          # A = A + A_init
  add s7, s7, t4          # B = B + B_init
  add s8, s8, t5          # C = C + C_init
  add s9, s9, t6          # D = D + D_init

  # Deallocate stack space
  addi sp, sp, 12
  ret                     # Return from single block processing

md5_done:
  # Store final hash digest (A, B, C, D) to output pointer (s3)
  # Store in little-endian byte order
  mv a2, s3               # Use a2 as the destination pointer

  # Store A (s6)
  sb s6, 0(a2)            # A[0]
  srli t0, s6, 8
  sb t0, 1(a2)            # A[1]
  srli t0, s6, 16
  sb t0, 2(a2)            # A[2]
  srli t0, s6, 24
  sb t0, 3(a2)            # A[3]
  addi a2, a2, 4         # Move pointer to B

  # Store B (s7)
  sb s7, 0(a2)            # B[0]
  srli t0, s7, 8
  sb t0, 1(a2)            # B[1]
  srli t0, s7, 16
  sb t0, 2(a2)            # B[2]
  srli t0, s7, 24
  sb t0, 3(a2)            # B[3]
  addi a2, a2, 4         # Move pointer to C

  # Store C (s8)
  sb s8, 0(a2)            # C[0]
  srli t0, s8, 8
  sb t0, 1(a2)            # C[1]
  srli t0, s8, 16
  sb t0, 2(a2)            # C[2]
  srli t0, s8, 24
  sb t0, 3(a2)            # C[3]
  addi a2, a2, 4         # Move pointer to D

  # Store D (s9)
  sb s9, 0(a2)            # D[0]
  srli t0, s9, 8
  sb t0, 1(a2)            # D[1]
  srli t0, s9, 16
  sb t0, 2(a2)            # D[2]
  srli t0, s9, 24
  sb t0, 3(a2)            # D[3]

  # Restore saved registers
  lw   ra, 124(sp)
  lw   s0, 120(sp)
  lw   s1, 116(sp)
  lw   s2, 112(sp)
  lw   s3, 108(sp)
  lw   s4, 104(sp)
  lw   s5, 100(sp)
  lw   s6, 96(sp)
  lw   s7, 92(sp)
  lw   s8, 88(sp)
  lw   s9, 84(sp)
  lw   s10, 80(sp)
  lw   s11, 76(sp)

  # Deallocate stack frame
  addi sp, sp, 128
  ret

# Remove the old md5_pad function as it's now integrated
# .globl md5_pad # (Keep if needed elsewhere, but likely not)
# md5_pad:
#   ... (old code removed) ...
#   ret

# -----------------------------------------------------------------
# Function: print_message_digest
#
# Description:
#   Prints the 16-byte MD5 message digest in a human-readable hexadecimal format.
#   Each byte of the digest is converted to its two-digit hexadecimal representation,
#   resulting in a 32-character string that represents the hash.
#
# Parameters:
#   a0 - Pointer to the MD5 digest.
# -----------------------------------------------------------------
  .globl print_message_digest
print_message_digest:
  addi sp, sp, -20
  sw ra, 16(sp)
  sw s0, 12(sp)
  sw s1, 8(sp)
  sw s2, 4(sp)
  sw s3, 0(sp)

  mv s0, a0
  li s1, 0   #count word

processWord:
  li t0, 4
  bge s1, t0, end
  lw s2, 0(s0)
  li s3, 0  #count byte

processByte:
  li t0, 4
  bge s3, t0, nextWord
  slli t0, s3, 3
  srl t1, s2, t0
  andi t1, t1, 0xFF

  srli a1, t1, 4
  jal ret_hex
  jal print_char
  andi a1, t1, 0x0F
  jal ret_hex
  jal print_char
  addi s3, s3, 1
  j processByte


nextWord:
  addi s0, s0, 4
  addi s1, s1, 1
  j processWord

end:
  lw   ra, 16(sp)
  lw   s0, 12(sp)
  lw   s1, 8(sp)
  lw   s2, 4(sp)
  lw   s3, 0(sp)
  addi sp, sp, 20
  ret

ret_hex:
  li t0, 10
  blt a1, t0, is_digit
  addi a1, a1, -10
  addi a1, a1, 'a'
  ret

is_digit:
  addi a1, a1, '0'
  ret

print_char:
  li a0, 11
  ecall
  ret
