.text
# R-type Instructions Test
main:
    addi x1, x0, 10   # x1 = x0 + 10
    addi x2, x0, 20   # x2 = x0 + 20

    add x3, x1, x2    # x3 = x1 + x2 = 30
    sub x4, x2, x1    # x4 = x2 - x1 = 10
    sll x5, x1, x2    # x5 = x1 << (x2 & 0x1F) = 10 << 20 = 10485760
    slt x6, x1, x2    # x6 = (x1 < x2) ? 1 : 0 = 1
    sltu x7, x1, x2   # x7 = (x1 < x2 unsigned) ? 1 : 0 = 1
    xor x8, x1, x2    # x8 = x1 ^ x2 = 30
    srl x9, x2, x1    # x9 = x2 >> (x1 & 0x1F) (logical) = 20 >> 10 = 0
    sra x10, x2, x1   # x10 = x2 >> (x1 & 0x1F) (arithmetic) = 20 >> 10 = 0
    or x11, x1, x2    # x11 = x1 | x2 = 30
    and x12, x1, x2   # x12 = x1 & x2 = 0
    ecall              # ecall to exit
