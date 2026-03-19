#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include "device.hpp"
#include <unordered_map>
#include <utility>


using namespace std;

class SystemBus : public Device {
public:

    SystemBus() {
        ;
    }

    uint32_t read(size_t address) const override {
        for (const auto &entry : device_map) {
            size_t start = entry.first.first;
            size_t length = entry.first.second;
            Device *device = entry.second;
            if (address >= start && address < start + length) {
                return device->read(address);
            }
        }
        std::cout << "Read from unmapped address: 0x" << hex << address << dec << endl;
        throw std::out_of_range("Address not found in any device");
    }

    void write(size_t address, uint32_t value, uint32_t mask) override {
        for (const auto &entry : device_map) {
            size_t start = entry.first.first;
            size_t length = entry.first.second;
            Device *device = entry.second;
            if (address >= start && address < start + length) {
                device->write(address, value, mask);
                return;
            }
        }
        std::cout << "Write to unmapped address: 0x" << hex << address << dec << endl;
        throw std::out_of_range("Address not found in any device");
    }

    void attach_device(size_t start_address, size_t length, Device *device) {
        device_map[{start_address, length}] = device;
    }

    void list_devices() const {
        cout << "================================================" << endl;
        cout << "Attached devices:" << endl;
        for (const auto &entry : device_map) {
            size_t start = entry.first.first;
            size_t length = entry.first.second;
            cout << "Device at address range [0x" << hex << start << ", 0x" << (start + length - 1) << dec << "]" << endl;
        }
        cout << "================================================" << endl;
    }

private:
    struct AddressRangeHash {
        size_t operator()(const std::pair<size_t, size_t>& range) const {
            return std::hash<size_t>()(range.first) ^ (std::hash<size_t>()(range.second) << 1);
        }
    };

    // Map of address ranges to devices (e.g., memory, UART, etc.)
    std::unordered_map<std::pair<size_t, size_t>, Device*, AddressRangeHash> device_map;  // {start_address, length} -> device
};
