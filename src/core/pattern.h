#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace nocturne::core {

class Process;

class Pattern {
public:
    struct ScanResult {
        uintptr_t address;
        bool found;
    };

    static std::vector<uint8_t> parse(const std::string& pattern);
    static ScanResult scan(Process* process, uintptr_t start, size_t size, const std::string& pattern);
    static ScanResult scan_module(Process* process, const std::wstring& moduleName, const std::string& pattern);

private:
    static bool compare(const uint8_t* data, const uint8_t* pattern, const uint8_t* mask, size_t len);
};

} // namespace nocturne::core
