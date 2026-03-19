// memory.hpp
#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include "device.hpp"


using namespace std;

class Memory : public Device {
public:

    Memory() : mem(nullptr), size(0), base_address(0) {}

    Memory(size_t base, size_t size) : size(size), base_address(base) {
        mem = new uint8_t[size];
        memset(mem, 0, size);
    }

    ~Memory() {
        delete[] mem;
    }

    // load from memory (little-endian), to cpu
    uint32_t read(size_t address) const {
        size_t offset = get_offset(address);
        if (offset + 4 > size) {
            throw runtime_error("Memory load_word out of bounds");
        }

        uint32_t value = 0;

        for(size_t i = 0; i < 4; i++) {
            value |= (uint32_t)mem[offset + i] << (i * 8);
        }

        return value;
    }

    // store to memory (little-endian), from cpu
    void write(size_t address, uint32_t value, uint32_t mask = 0xF) {
        size_t offset = get_offset(address);
        if (offset + 4 > size) {
            throw runtime_error("Memory store_word out of bounds");
        }

        for(size_t i = 0; i < 4; i++) {
            if (mask & (1 << i)) {
                mem[offset + i] = (uint8_t)(value >> (i * 8)) & 0xFF;
            }
        }
    }

private:
    size_t get_offset(size_t address) const {
        if (address < base_address || address >= base_address + size) {
            throw runtime_error("Memory access out of bounds");
        }
        return address - base_address;
    }
    uint8_t *mem;
    size_t size;
    size_t base_address;
};

// 0x11223344, 0x3

// 0x3 & 0x1 = 1 (true), 0x11223344 >> 0 = 0x11223344 & 0xFF = 0x44 store to address 0
// 0x3 & 0x2 = 2 (true), 0x11223344 >> 8 = 0x112233 & 0xFF = 0x33 strore to address 1
// 0x3 & 0x4 = 0 (false), 0x11223344 >> 16 = 0x1122 & 0xFF = 0x22 (not stored)
// 0x3 & 0x8 = 0 (false), 0x11223344 >> 24 = 0x11 & 0xFF = 0x11 (not stored)


// origin: 0x55667788
// store: 0x11223344, 0x3 (0b0011)

// 0x55667788 & 0xFFFF0000 = 0x55660000 (origin & ~mask)
// 0x11223344 & 0x0000FFFF = 0x00003344 (store & mask)

// final: 0x55663344 (origin & ~mask) | (store & mask)