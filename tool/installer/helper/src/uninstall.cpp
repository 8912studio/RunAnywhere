#include "uninstall.h"
#include <Windows.h>
#include <RestartManager.h>

namespace {

void Terminate(DWORD session_handle, const std::filesystem::path& exe_path) {

    const wchar_t* files[] = {
        exe_path.c_str()
    };

    DWORD error = RmRegisterResources(
        session_handle,
        std::size(files),
        files,
        0, 
        nullptr, 
        0,
        nullptr);

    if (error) {
        return;
    }

    RmShutdown(session_handle, RmForceShutdown, nullptr);
}

}


void TerminateApplication(const std::filesystem::path& exe_path) {

    DWORD session_handle{};
    wchar_t session_key[64]{};
    DWORD error = RmStartSession(&session_handle, 0, session_key);
    if (error) {
        return;
    }

    Terminate(session_handle, exe_path);
    RmEndSession(session_handle);
}