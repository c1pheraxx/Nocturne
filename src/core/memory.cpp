#include "memory.h"
#include "process.h"

namespace nocturne::core {

bool Memory::read(uintptr_t address, void* buffer, size_t size) const {
    if (!process_ || !process_->is_attached()) return false;
    SIZE_T read = 0;
    return ReadProcessMemory(process_->handle(), reinterpret_cast<LPCVOID>(address), buffer, size, &read) && read == size;
}

std::string Memory::read_string(uintptr_t address, size_t maxLen) const {
    std::string result;
    result.reserve(maxLen);
    char ch;
    for (size_t i = 0; i < maxLen; ++i) {
        if (!read(address + i, &ch, 1) || ch == '\0') break;
        result.push_back(ch);
    }
    return result;
}

std::wstring Memory::read_wstring(uintptr_t address, size_t maxLen) const {
    std::wstring result;
    result.reserve(maxLen);
    wchar_t ch;
    for (size_t i = 0; i < maxLen; ++i) {
        if (!read(address + i * sizeof(wchar_t), &ch, sizeof(wchar_t)) || ch == L'\0') break;
        result.push_back(ch);
    }
    return result;
}

uintptr_t Memory::read_chain(uintptr_t base, const std::vector<uintptr_t>& offsets) const {
    uintptr_t addr = read<uintptr_t>(base);
    for (size_t i = 0; i < offsets.size() - 1; ++i) {
        if (addr == 0) return 0;
        addr = read<uintptr_t>(addr + offsets[i]);
    }
    if (addr == 0) return 0;
    return addr + offsets.back();
}

} // namespace nocturne::core
