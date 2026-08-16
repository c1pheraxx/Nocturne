#pragma once
#include <windows.h>
#include <string>
#include <cstdint>

namespace nocturne::core {

struct ModuleInfo {
    std::wstring name;
    uintptr_t base;
    size_t size;
};

class Module {
public:
    static ModuleInfo find(uint32_t pid, const std::wstring& moduleName);
};

} // namespace nocturne::core
