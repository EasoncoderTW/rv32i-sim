#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include "device.hpp"

using namespace std;

class Uart : public Device {
public:
    Uart() = default;

    uint32_t read(size_t address) const override {
        // For simplicity, we can ignore the address and just return 0
        return 0;
    }

    void write(size_t address, uint32_t value, uint32_t mask) override {
        // For simplicity, we can ignore the address and mask, and just print the value
        char c = static_cast<char>(value & 0xFF); // Only consider the least significant byte
        printf("%c", c); // Print the character to the console
        fflush(stdout); // Ensure the output is flushed immediately
    }

};