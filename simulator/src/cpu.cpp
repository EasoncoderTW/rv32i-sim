#include "cpu.hpp"
#include "utils.hpp"
#include <iostream>

using namespace std;

/* constructor - reset cpu */
RV32ICPU::RV32ICPU() : pc(0) {
    // Initialize registers to zero
    for (int i = 0; i < 32; ++i) {
        registers[i] = 0;
    }
    instruction_count = 0;
}

void RV32ICPU::run() {
    bool running = true;
    while (running) {
        // Fetch instruction
        uint32_t instructions = device_master->read(pc);
        instruction_count++;

        // Decode instruction
        Instruction instr = decode_instruction(instructions);

        uint32_t next_pc = pc + 4; // default pc increment for 32bits (4 bytes)

        // Opreration handling (example for add instruction)
        switch (instr.operation) {
            // R-type instructions
            case OPERATION::ADD:
                registers[instr.rd] = registers[instr.rs1] + registers[instr.rs2];
                break;
            case OPERATION::SUB:
                registers[instr.rd] = registers[instr.rs1] - registers[instr.rs2];
                break;
            case OPERATION::SLL:
                registers[instr.rd] = registers[instr.rs1] << instr.shamt;
                break;
            case OPERATION::SLT: // signed comparison
                registers[instr.rd] = (int32_t(registers[instr.rs1]) < int32_t(registers[instr.rs2])) ? 1 : 0;
                break;
            case OPERATION::SLTU: // unsigned comparison
                registers[instr.rd] = (registers[instr.rs1] < registers[instr.rs2]) ? 1 : 0;
                break;
            case OPERATION::XOR:
                registers[instr.rd] = registers[instr.rs1] ^ registers[instr.rs2];
                break;
            case OPERATION::SRL:
                registers[instr.rd] = registers[instr.rs1] >> instr.shamt; // logical right shift
                break;
            case OPERATION::SRA:
                registers[instr.rd] = int32_t(registers[instr.rs1]) >> instr.shamt; // arithmetic right shift
                break;
            case OPERATION::OR:
                registers[instr.rd] = registers[instr.rs1] | registers[instr.rs2];
                break;
            case OPERATION::AND:
                registers[instr.rd] = registers[instr.rs1] & registers[instr.rs2];
                break;
            // I-type instructions
            case OPERATION::ADDI:
                registers[instr.rd] = registers[instr.rs1] + instr.imm;
                break;
            case OPERATION::SLTI: // signed comparison
                registers[instr.rd] = (int32_t(registers[instr.rs1]) < int32_t(instr.imm)) ? 1 : 0;
                break;
            case OPERATION::SLTIU: // unsigned comparison
                registers[instr.rd] = (registers[instr.rs1] < uint32_t(instr.imm)) ? 1 : 0;
                break;
            case OPERATION::XORI:
                registers[instr.rd] = registers[instr.rs1] ^ instr.imm;
                break;
            case OPERATION::ORI:
                registers[instr.rd] = registers[instr.rs1] | instr.imm;
                break;
            case OPERATION::ANDI:
                registers[instr.rd] = registers[instr.rs1] & instr.imm;
                break;
            case OPERATION::SLLI:
                registers[instr.rd] = registers[instr.rs1] << (instr.imm & 0x1F); // at most shift by 31
                break;
            case OPERATION::SRLI:
                registers[instr.rd] = registers[instr.rs1] >> (instr.imm & 0x1F); // logical right shift
                break;
            case OPERATION::SRAI:
                registers[instr.rd] = int32_t(registers[instr.rs1]) >> (instr.imm & 0x1F); // arithmetic right shift
                break;

            // B-type instructions (branch)
            case OPERATION::BEQ:
                if (registers[instr.rs1] == registers[instr.rs2]) next_pc = pc + instr.imm;
                break;
            case OPERATION::BNE:
                if (registers[instr.rs1] != registers[instr.rs2]) next_pc = pc + instr.imm;
                break;
            case OPERATION::BLT:
                if (int32_t(registers[instr.rs1]) < int32_t(registers[instr.rs2])) next_pc = pc + instr.imm;
                break;
            case OPERATION::BGE:
                if (int32_t(registers[instr.rs1]) >= int32_t(registers[instr.rs2])) next_pc = pc + instr.imm;
                break;
            case OPERATION::BLTU:
                if (registers[instr.rs1] < registers[instr.rs2]) next_pc = pc + instr.imm;
                break;
            case OPERATION::BGEU:
                if (registers[instr.rs1] >= registers[instr.rs2]) next_pc = pc + instr.imm;
                break;

            // Load and Store instructions
            case OPERATION::LW:
                registers[instr.rd] = device_master->read(registers[instr.rs1] + instr.imm);
                break;
            case OPERATION::LH:
                registers[instr.rd] = sign_extend(device_master->read(registers[instr.rs1] + instr.imm) & 0xFFFF, 16);
                break;
            case OPERATION::LB:
                registers[instr.rd] = sign_extend(device_master->read(registers[instr.rs1] + instr.imm) & 0xFF, 8);
                break;
            case OPERATION::LHU:
                registers[instr.rd] = device_master->read(registers[instr.rs1] + instr.imm) & 0xFFFF;
                break;
            case OPERATION::LBU:
                registers[instr.rd] = device_master->read(registers[instr.rs1] + instr.imm) & 0xFF;
                break;
            case OPERATION::SW:
                device_master->write(registers[instr.rs1] + instr.imm, registers[instr.rs2], 0xF);
                break;
            case OPERATION::SH:
                device_master->write(registers[instr.rs1] + instr.imm, registers[instr.rs2], 0x3);
                break;
            case OPERATION::SB:
                device_master->write(registers[instr.rs1] + instr.imm, registers[instr.rs2], 0x1);
                break;

            // U-type instructions
            case OPERATION::LUI:
                registers[instr.rd] = instr.imm; // Load Upper Immediate (upper 20 bits [31:12])
                break;
            case OPERATION::AUIPC:
                registers[instr.rd] = pc + instr.imm; // Add Upper Immediate
                break;

            // J-type instructions
            case OPERATION::JAL:
                registers[instr.rd] = pc + 4; // store return address (link)
                next_pc = (pc + instr.imm) & 0xFFFFFFFC; // jump to target address
                break;
            case OPERATION::JALR:
                registers[instr.rd] = pc + 4; // store return address (link)
                next_pc = (registers[instr.rs1] + instr.imm) & 0xFFFFFFFC; // jump to target address
                break;

            // ECALL instruction to stop execution
            case OPERATION::ECALL:
                running = false;
                break;

            // Unrecognized instruction
            default:
                cerr << "Unrecognized instruction: " << instr.opcode << endl;
                running = false;
                break;
        }
        // x0 always zero
        registers[0] = 0;

        // update pc and handle termination condition
        pc = next_pc;
    }
}

void RV32ICPU::dump_registers(bool hexadecimal) {
    cout << "================================================" << endl;
    cout << "|                Register state                |" << endl;
    cout << "================================================" << endl;

    for (int i = 0; i < 32; ++i) {
        printf("x%-2d: ", i);
        if (hexadecimal) { // print in hexadecimal
            printf("0x%08X\t", registers[i]);
        } else { // print in decimal
            printf("%10u\t", registers[i]);
        }
        if ((i + 1) % 8 == 0) cout << endl;
    }

    cout << "Base: " << (hexadecimal ? "Hexadecimal" : "Decimal") << endl;
    cout << "================================================" << endl;
}