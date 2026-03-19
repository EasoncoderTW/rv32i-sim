#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <fstream>

using namespace std;

enum class OPERATION {
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    LW, LH, LB, LHU, LBU , SW, SH, SB,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    LUI, AUIPC, JAL, JALR,
    ECALL,
    UNKNOWN
};


struct Instruction {
    uint32_t instruction_word; // raw 32-bit instruction
    uint32_t opcode; // opcode field
    uint32_t rd; // destination register
    uint32_t rs1; // source register 1
    uint32_t rs2; // source register 2
    uint32_t imm; // immediate value
    uint32_t shamt; // shift amount for shift instructions
    OPERATION operation; // decoded operation
};

uint32_t sign_extend(uint32_t value, int bits);
Instruction decode_instruction(uint32_t instruction_word);

// Memory size convertion (e.g., 128K -> 131072)
size_t parse_memory_size(const string &size_str);