#pragma once
#include <windows.h>
#include <cstdint>
#include <vector>
#include <string>

namespace nocturne::core {

class Process;

class Memory {
public:
    explicit Memory(Process* process) : process_(process) {}

    bool read(uintptr_t address, void* buffer, size_t size) const;

    template<typename T>
    T read(uintptr_t address) const {
        T value{};
        read(address, &value, sizeof(T));
        return value;
    }

    std::string read_string(uintptr_t address, size_t maxLen = 256) const;
    std::wstring read_wstring(uintptr_t address, size_t maxLen = 256) const;

    uintptr_t read_chain(uintptr_t base, const std::vector<uintptr_t>& offsets) const;

private:
    Process* process_;
};

} // namespace nocturne::core
