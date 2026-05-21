#pragma once
#include <iostream>
#include "csr.hpp"
#include "utils.hpp"
#include "device.hpp"
#include "timer.hpp"

using namespace std;

class RV32ICPU {
public:
    RV32ICPU();

    void bind_device(Device *dev) {
        device_master = dev;
    }

    void bind_timer(Timer *timer_device) {
        timer = timer_device;
    }

    bool tick();
    void set_pc(uint32_t address) {
        pc = address;
    }

    // heller function
    void dump_registers(bool hexadecimal);
    void dump_instruction_count() {
        cout << "Total instructions executed: " << instruction_count << endl;
    }

private:
    bool handle_interrupts();
    bool execute_csr_instruction(const Instruction &instr);

    uint32_t registers[32]; // 32 general-purpose registers
    uint32_t pc; // program counter

    CsrFile csr_file;

    Device *device_master; // interface
    Timer *timer; // timer device

    // for debug use
    uint32_t instruction_count = 0;
};