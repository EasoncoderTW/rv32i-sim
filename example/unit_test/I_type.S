.text
# I-type Instructions Test
main:
    addi x1, x0, 10   # x1 = x0 + 10
    addi x2, x0, 20   # x2 = x0 + 20

    slti x3, x1, x2   # x3 = (x1 < x2) ? 1 : 0 = 1
    sltiu x4, x2, x1  # x4 = (x2 < x1 unsigned) ? 1 : 0 = 0
    xori x5, x1, 15   # x5 = x1 ^ 15 = 5
    ori  x6, x1, 12   # x6 = x1 | 12 = 14
    andi x7, x2, 25   # x7 = x2 & 25 = 16
    slli x8, x1, 2    # x8 = x1 << 2 = 40
    srli x9, x2, 1    # x9 = x2 >> 1 (logical) = 10
    srai x10, x2, 1   # x10 = x2 >> 1 (arithmetic) = 10
    ecall              # ecall to exit