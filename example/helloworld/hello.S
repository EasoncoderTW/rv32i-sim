.text
.global main
main:
    la a0, message # load address of message into a0
    li a1, 0x400 # load address of uart into a1
loop:
    lbu a2, (a0)  # load byte (unsigned) from message into a2
    beq a2, zero, end # if a2 is zero (end of string), jump to end
    sb a2, (a1) # store byte from a2 to uart
    addi a0, a0, 1 # increment a0 to point to the next byte in message
    j loop
end:
    ecall

.data
message:
    .ascii "hello,world\n"
    .byte 0
