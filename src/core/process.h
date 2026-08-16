#pragma once
#include <windows.h>
#include <string>
#include <cstdint>

namespace nocturne::core {

class Process {
public:
    Process() = default;
    ~Process();

    bool attach(const std::wstring& processName);
    bool attach(uint32_t pid);
    void detach();

    bool is_attached() const { return handle_ != nullptr && handle_ != INVALID_HANDLE_VALUE; }
    uint32_t pid() const { return pid_; }
    HANDLE handle() const { return handle_; }

    std::wstring name() const { return name_; }

private:
    HANDLE handle_ = nullptr;
    uint32_t pid_ = 0;
    std::wstring name_;
};

} // namespace nocturne::core
