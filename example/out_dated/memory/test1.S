.text
.global main
main:
    li x5, 10          # Load immediate value 10 into register x5
    li x6, 20          # Load immediate value 20 into register x6
    add x7, x5, x6     # Add values in x5 and x6, store result in x7
    la x1, result      # Load address of 'result' into register x1
    sw x7, 0(x1)       # Store the result from x7 into memory at address in x1
    lw x8, 0(x1)       # Load the value from memory address in x1 into register x8
    ecall

.data
    .align 4
result:
    .word 0x0         # Reserve a word in memory initialized to 0
    .byte 0x12
    .half 0x3456


# bitwidth - 32bits (4 bytes), mask (4bits) to indicate which bytes are being written
# DRAM bitwidth - 256 bits (32 bytes), mask (32bits) to indicate which bytes are being written

# x1 = 0xff (255)
# x7 = 0x10 (16)

# sb x7, 0(x1)   # Store byte from x7 into memory at address x1 (0xff)
# "sb" -> Store Byte: mask=0x1 (0b_0001)
# "x7" -> 0x10 (16): value to store
# "0(x1)" -> 0xff + 0 (255): address for storing

# *(0xff) = 0x10  # Memory at address 0xff now contains 0x10


# x2 = 0x80 (128)
# x8 = 0x20 (32)
# sw x8, 4(x2)   # Store word from x8 into memory at address x2 + 4 (0x84)
# "sw" -> Store Word: mask=0xf (0b_1111)
# "x8" -> 0x20 (32): value to store
# "4(x2)" -> 0x80 + 4 (132): address for storing

# *(0x80 + 4) = 0x20  # Memory at address 0x84 now contains 0x20

# # sb (1 byte), sh (2 bytes), sw (4 bytes)

# # x  : xxxx (0000) -> 0x00 (0)
# # sb : xxxv (0001) -> 0x01 (1)
# # sh : xxvv (0011) -> 0x03 (3)
# # sw : vvvv (1111) -> 0x0f (15)

# 1 word = 4 bytes = 32 bits
# mask use 4 bits to indicate which bytes are being written:
