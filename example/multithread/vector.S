// rv32i_zicsr
.set CONTEXT_SIZE, 128
.set CTX_RA, 0
.set CTX_GP, 4
.set CTX_TP, 8
.set CTX_T0, 12
.set CTX_T1, 16
.set CTX_T2, 20
.set CTX_S0, 24
.set CTX_S1, 28
.set CTX_A0, 32
.set CTX_A1, 36
.set CTX_A2, 40
.set CTX_A3, 44
.set CTX_A4, 48
.set CTX_A5, 52
.set CTX_A6, 56
.set CTX_A7, 60
.set CTX_S2, 64
.set CTX_S3, 68
.set CTX_S4, 72
.set CTX_S5, 76
.set CTX_S6, 80
.set CTX_S7, 84
.set CTX_S8, 88
.set CTX_S9, 92
.set CTX_S10, 96
.set CTX_S11, 100
.set CTX_T3, 104
.set CTX_T4, 108
.set CTX_T5, 112
.set CTX_T6, 116
.set CTX_MEPC, 120

.text
.global interrupt_handler
.global hardware_timer_interrupt_handler
.global task_restore_first

interrupt_handler:
    addi sp, sp, -CONTEXT_SIZE
    sw ra, CTX_RA(sp)
    sw gp, CTX_GP(sp)
    sw tp, CTX_TP(sp)
    sw t0, CTX_T0(sp)
    sw t1, CTX_T1(sp)
    sw t2, CTX_T2(sp)
    sw s0, CTX_S0(sp)
    sw s1, CTX_S1(sp)
    sw a0, CTX_A0(sp)
    sw a1, CTX_A1(sp)
    sw a2, CTX_A2(sp)
    sw a3, CTX_A3(sp)
    sw a4, CTX_A4(sp)
    sw a5, CTX_A5(sp)
    sw a6, CTX_A6(sp)
    sw a7, CTX_A7(sp)
    sw s2, CTX_S2(sp)
    sw s3, CTX_S3(sp)
    sw s4, CTX_S4(sp)
    sw s5, CTX_S5(sp)
    sw s6, CTX_S6(sp)
    sw s7, CTX_S7(sp)
    sw s8, CTX_S8(sp)
    sw s9, CTX_S9(sp)
    sw s10, CTX_S10(sp)
    sw s11, CTX_S11(sp)
    sw t3, CTX_T3(sp)
    sw t4, CTX_T4(sp)
    sw t5, CTX_T5(sp)
    sw t6, CTX_T6(sp)

    csrr t0, mepc
    sw t0, CTX_MEPC(sp)

    csrr t0, mcause
    li t1, 0x80000007
    beq t0, t1, hardware_timer_interrupt_handler
    j restore_context

hardware_timer_interrupt_handler:
    mv a0, sp
    call task_schedule_from_interrupt
    mv sp, a0

restore_context:
    lw ra, CTX_RA(sp)
    lw gp, CTX_GP(sp)
    lw tp, CTX_TP(sp)
    lw s0, CTX_S0(sp)
    lw s1, CTX_S1(sp)
    lw a0, CTX_A0(sp)
    lw a1, CTX_A1(sp)
    lw a2, CTX_A2(sp)
    lw a3, CTX_A3(sp)
    lw a4, CTX_A4(sp)
    lw a5, CTX_A5(sp)
    lw a6, CTX_A6(sp)
    lw a7, CTX_A7(sp)
    lw s2, CTX_S2(sp)
    lw s3, CTX_S3(sp)
    lw s4, CTX_S4(sp)
    lw s5, CTX_S5(sp)
    lw s6, CTX_S6(sp)
    lw s7, CTX_S7(sp)
    lw s8, CTX_S8(sp)
    lw s9, CTX_S9(sp)
    lw s10, CTX_S10(sp)
    lw s11, CTX_S11(sp)
    lw t3, CTX_T3(sp)
    lw t4, CTX_T4(sp)
    lw t5, CTX_T5(sp)
    lw t6, CTX_T6(sp)
    lw t2, CTX_MEPC(sp)
    csrw mepc, t2
    lw t0, CTX_T0(sp)
    lw t1, CTX_T1(sp)
    lw t2, CTX_T2(sp)
    addi sp, sp, CONTEXT_SIZE
    mret

task_restore_first:
    mv sp, a0
    j restore_context
