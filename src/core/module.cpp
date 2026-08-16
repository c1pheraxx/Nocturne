#include "module.h"
#include <tlhelp32.h>

namespace nocturne::core {

ModuleInfo Module::find(uint32_t pid, const std::wstring& moduleName) {
    ModuleInfo info{};
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (snap == INVALID_HANDLE_VALUE) return info;

    MODULEENTRY32W me;
    me.dwSize = sizeof(me);

    if (Module32FirstW(snap, &me)) {
        do {
            if (moduleName == me.szModule) {
                info.name = me.szModule;
                info.base = reinterpret_cast<uintptr_t>(me.modBaseAddr);
                info.size = me.modBaseSize;
                break;
            }
        } while (Module32NextW(snap, &me));
    }

    CloseHandle(snap);
    return info;
}

} // namespace nocturne::core
