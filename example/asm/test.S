.text
main:
    addi x5, x0, 10   # x5 = 10
    addi x6, x0, 20   # x6 = 20
    add  x7, x5, x6   # x7 = x5 + x6 = 30
    ecall             # ecall to exit
