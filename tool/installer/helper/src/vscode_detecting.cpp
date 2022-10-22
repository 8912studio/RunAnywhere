#include "vscode_detecting.h"
#include <Windows.h>

namespace {

bool Detect() {

    wchar_t command_line[] = LR"(cmd.exe /c "code --version")";

    STARTUPINFO startup_info{};
    startup_info.cb = sizeof(startup_info);

    PROCESS_INFORMATION process_info{};

    BOOL is_succeeded = CreateProcess(
        nullptr, 
        command_line, 
        nullptr,
        nullptr,
        FALSE,
        CREATE_NO_WINDOW, 
        nullptr,
        nullptr,
        &startup_info,
        &process_info);

    if (!is_succeeded) {
        return false;
    }

    WaitForSingleObject(process_info.hProcess, INFINITE);

    DWORD exit_code{};
    is_succeeded = GetExitCodeProcess(process_info.hProcess, &exit_code);

    CloseHandle(process_info.hThread);
    CloseHandle(process_info.hProcess);

    return is_succeeded && (exit_code == 0);
}

}


bool DetectIfVSCodeInstalled() {

    static const auto result = Detect();
    return result;
}