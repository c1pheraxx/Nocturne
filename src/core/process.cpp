#include "process.h"
#include <tlhelp32.h>

namespace nocturne::core {

Process::~Process() {
    detach();
}

bool Process::attach(const std::wstring& processName) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);

    if (Process32FirstW(snap, &pe)) {
        do {
            if (processName == pe.szExeFile) {
                pid_ = pe.th32ProcessID;
                name_ = pe.szExeFile;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }

    CloseHandle(snap);

    if (pid_ == 0) return false;
    return attach(pid_);
}

bool Process::attach(uint32_t pid) {
    detach();
    pid_ = pid;
    handle_ = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid_);
    return is_attached();
}

void Process::detach() {
    if (handle_ && handle_ != INVALID_HANDLE_VALUE) {
        CloseHandle(handle_);
        handle_ = nullptr;
    }
    pid_ = 0;
}

} // namespace nocturne::core
