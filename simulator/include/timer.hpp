#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include "device.hpp"

using namespace std;

// MMIO (memory-mapped I/O)
class Timer : public Device {
private:
    uint64_t mtime = 0; // Current time in ticks
    uint64_t mtimecmp = 0xFFFFFFFFFFFFFFFFULL; // Time compare value for generating interrupts

    static constexpr size_t MTIMECMP_OFFSET = 0x4000; // Offset for mtimecmp register
    static constexpr size_t MTIME_OFFSET = 0xBFF8; // Offset for mtime register

    bool interrupt_pending = false; // Flag to indicate if an interrupt is pending

    void update_interrupt_pending() {
        interrupt_pending = mtime >= mtimecmp;
    }

public:
    Timer() = default;
    uint32_t read(size_t address) const override {
        address &= 0xFFFF; // Mask the address to fit within the timer's address space
        if (address == MTIME_OFFSET) {
            return static_cast<uint32_t>(mtime & 0xFFFFFFFF); // Return lower 32 bits of mtime
        } else if (address == MTIME_OFFSET + 0x4) {
            return static_cast<uint32_t>((mtime >> 32) & 0xFFFFFFFF); // Return upper 32 bits of mtime
        } else if (address == MTIMECMP_OFFSET) {
            return static_cast<uint32_t>(mtimecmp & 0xFFFFFFFF); // Return lower 32 bits of mtimecmp
        } else if (address == MTIMECMP_OFFSET + 0x4) {
            return static_cast<uint32_t>((mtimecmp >> 32) & 0xFFFFFFFF); // Return upper 32 bits of mtimecmp
        } else {
            char error_msg[100];
            snprintf(error_msg, sizeof(error_msg), "Invalid address for Timer read: 0x%zx", address);
            throw std::out_of_range(error_msg);
        }
    }

    void write(size_t address, uint32_t value, uint32_t mask) override {
        (void)mask;
        address &= 0xFFFF; // Mask the address to fit within the timer's address space
        if (address == MTIME_OFFSET) {
            mtime = (mtime & 0xFFFFFFFF00000000) | (value & 0xFFFFFFFF); // Update lower 32 bits of mtime
        } else if (address == MTIME_OFFSET + 0x4) {
            mtime = (mtime & 0x00000000FFFFFFFF) | (static_cast<uint64_t>(value) << 32); // Update upper 32 bits of mtime
        } else if (address == MTIMECMP_OFFSET) {
            mtimecmp = (mtimecmp & 0xFFFFFFFF00000000) | (value & 0xFFFFFFFF); // Update lower 32 bits of mtimecmp
        } else if (address == MTIMECMP_OFFSET + 0x4) {
            mtimecmp = (mtimecmp & 0x00000000FFFFFFFF) | (static_cast<uint64_t>(value) << 32); // Update upper 32 bits of mtimecmp
        } else {
            char error_msg[100];
            snprintf(error_msg, sizeof(error_msg), "Invalid address for Timer write: 0x%zx", address);
            throw std::out_of_range(error_msg);
        }

        update_interrupt_pending();
    }

    // Method to simulate the passage of time
    void tick() {
        mtime++; // Increment the current time by one tick
        update_interrupt_pending();
    }

    void clear_interrupt() {
        interrupt_pending = false; // Clear the interrupt pending flag
    }

    bool is_interrupt_pending() const {
        return interrupt_pending; // Return the status of the interrupt pending flag
    }

};