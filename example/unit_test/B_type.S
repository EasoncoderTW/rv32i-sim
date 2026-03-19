.text
# I-type Instructions Test
main:
    add x2, x0, x0 # init x2 = 0
    addi x3, x0, 5 # target x3 = 5
loop_1:
    addi x2, x2, 1
    blt x2, x3, loop_1  # branch if x2 < x3
    ecall              # ecall to exit

lui x2, 0x12345 # 20bits
addi x2, x2, 0x678 # 12bits

x2=0x12345678
