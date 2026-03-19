// device.hpp
#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <stdexcept>



// Abstract base class for devices (e.g., memory-mapped I/O)
class Device {
public:
    virtual ~Device() = default;
    virtual uint32_t read(size_t address) const = 0; // Read from device
    virtual void write(size_t address, uint32_t value, uint32_t mask) = 0; // Write
};
