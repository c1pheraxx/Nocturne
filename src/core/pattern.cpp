#include "pattern.h"
#include "process.h"
#include "module.h"
#include "memory.h"
#include <algorithm>

namespace nocturne::core {

std::vector<uint8_t> Pattern::parse(const std::string& pattern) {
    std::vector<uint8_t> bytes;
    std::string current;
    for (char c : pattern) {
        if (c == ' ') {
            if (!current.empty()) {
                bytes.push_back(static_cast<uint8_t>(std::stoul(current, nullptr, 16)));
                current.clear();
            }
        } else if (c == '?') {
            bytes.push_back(0x00); // placeholder, mask handles it
            current.clear();
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        bytes.push_back(static_cast<uint8_t>(std::stoul(current, nullptr, 16)));
    }
    return bytes;
}

bool Pattern::compare(const uint8_t* data, const uint8_t* pattern, const bool* mask, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (!mask[i] && data[i] != pattern[i]) return false;
    }
    return true;
}

Pattern::ScanResult Pattern::scan(Process* process, uintptr_t start, size_t size, const std::string& pat) {
    ScanResult result{0, false};
    if (!process || !process->is_attached()) return result;

    auto bytes = parse(pat);
    if (bytes.empty()) return result;

    std::vector<bool> mask;
    mask.reserve(bytes.size());
    {
        std::string current;
        for (char c : pat) {
            if (c == ' ') {
                if (!current.empty()) {
                    mask.push_back(true);
                    current.clear();
                }
            } else if (c == '?') {
                mask.push_back(false);
                current.clear();
            } else {
                current += c;
            }
        }
        if (!current.empty()) mask.push_back(true);
    }

    std::vector<uint8_t> buffer(size);
    SIZE_T read = 0;
    if (!ReadProcessMemory(process->handle(), reinterpret_cast<LPCVOID>(start), buffer.data(), size, &read)) {
        return result;
    }

    for (size_t i = 0; i <= read - bytes.size(); ++i) {
        if (compare(buffer.data() + i, bytes.data(), mask.data(), bytes.size())) {
            result.address = start + i;
            result.found = true;
            return result;
        }
    }

    return result;
}

Pattern::ScanResult Pattern::scan_module(Process* process, const std::wstring& moduleName, const std::string& pat) {
    auto info = Module::find(process->pid(), moduleName);
    if (info.base == 0) return {0, false};
    return scan(process, info.base, info.size, pat);
}

} // namespace nocturne::core
