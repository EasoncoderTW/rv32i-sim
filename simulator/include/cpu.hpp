#pragma once
#include <iostream>
#include "utils.hpp"
#include "device.hpp"

using namespace std;

class RV32ICPU {
public:
    RV32ICPU();

    void bind_device(Device *dev) {
        device_master = dev;
    }

    void run();
    void set_pc(uint32_t address) {
        pc = address;
    }

    // heller function
    void dump_registers(bool hexadecimal);

private:
    uint32_t registers[32]; // 32 general-purpose registers
    uint32_t pc; // program counter

    Device *device_master; // interface
};