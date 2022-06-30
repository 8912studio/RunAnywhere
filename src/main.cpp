#include <zaf/application.h>
#include <zaf/base/handle.h>
#include "application_delegate.h"
#include "ipc.h"
#include "resource.h"

namespace {

zaf::Handle g_exclusive_mutex;

bool CanApplicationRun() {

    g_exclusive_mutex.Reset(CreateMutex(nullptr, TRUE, L"Zplutor.RunAnywhere.Exclusive"));
    if (!g_exclusive_mutex) {
        return false;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        return false;
    }

    return true;
}


void SendCommandLineToOtherApplication(LPWSTR command_line) {

    std::size_t command_line_length = std::wcslen(command_line);
    if (command_line_length <= 0) {
        return;
    }

    HWND ipc_window = FindWindow(ra::IPCWindowClassName, nullptr);
    if (!ipc_window) {
        return;
    }

    COPYDATASTRUCT copy_data_info{};
    copy_data_info.dwData = ra::IPCMessageIdentifier;
    copy_data_info.cbData = static_cast<DWORD>(command_line_length) * sizeof(wchar_t);
    copy_data_info.lpData = command_line;

    SendMessage(ipc_window, WM_COPYDATA, {}, reinterpret_cast<LPARAM>(&copy_data_info));
}

}


int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR command_line, int) {

    if (!CanApplicationRun()) {

        SendCommandLineToOtherApplication(command_line);
        return 0;
    }

    zaf::InitializeParameters initialize_parameters;
    initialize_parameters.delegate = std::make_shared<ra::ApplicationDelegate>();
    initialize_parameters.window_icon = LoadIcon(
        GetModuleHandle(nullptr),
        MAKEINTRESOURCE(IDI_APPICON));

    auto& application = zaf::Application::Instance();
    application.Initialize(initialize_parameters);
    application.Run();
    return 0;
}