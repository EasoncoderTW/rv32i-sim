#pragma once

// OPCODE definitions for RV32I instructions

#define OPCODE_R_TYPE 0x33
#define OPCODE_I_TYPE 0x13
#define OPCODE_LOAD 0x03
#define OPCODE_STORE 0x23
#define OPCODE_BRANCH 0x63
#define OPCODE_JAL 0x6F
#define OPCODE_JALR 0x67
#define OPCODE_LUI 0x37
#define OPCODE_AUIPC 0x17
#define OPCODE_SYSTEM 0x73

#define ECALL_OPCODE 0x00000073
#define MRET_OPCODE 0x30200073

#define FUNCT3_CSRRW 0x1
#define FUNCT3_CSRRS 0x2
#define FUNCT3_CSRRC 0x3
#define FUNCT3_CSRRWI 0x5
#define FUNCT3_CSRRSI 0x6
#define FUNCT3_CSRRCI 0x7

#define FUNCT3_ADD_SUB 0x0
#define FUNCT3_SLL 0x1
#define FUNCT3_SLT 0x2
#define FUNCT3_SLTU 0x3
#define FUNCT3_XOR 0x4
#define FUNCT3_SRL_SRA 0x5
#define FUNCT3_OR 0x6
#define FUNCT3_AND 0x7

#define FUNCT3_ADDI 0x0
#define FUNCT3_SLTI 0x2
#define FUNCT3_SLTIU 0x3
#define FUNCT3_XORI 0x4
#define FUNCT3_ORI 0x6
#define FUNCT3_ANDI 0x7
#define FUNCT3_SLLI 0x1
#define FUNCT3_SRLI_SRAI 0x5

#define FUNCT3_LB 0x0
#define FUNCT3_LH 0x1
#define FUNCT3_LW 0x2
#define FUNCT3_LBU 0x4
#define FUNCT3_LHU 0x5

#define FUNCT3_SB 0x0
#define FUNCT3_SH 0x1
#define FUNCT3_SW 0x2

#define FUNCT3_BEQ 0x0
#define FUNCT3_BNE 0x1
#define FUNCT3_BLT 0x4
#define FUNCT3_BGE 0x5
#define FUNCT3_BLTU 0x6
#define FUNCT3_BGEU 0x7

#define FUNCT7_ADD 0x00
#define FUNCT7_SUB 0x20

#define FUNCT7_SRLI 0x00
#define FUNCT7_SRAI 0x20

#define FUNCT7_SRL 0x00
#define FUNCT7_SRA 0x20