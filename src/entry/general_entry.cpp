#include "entry/general_entry.h"
#include <zaf/application.h>
#include <zaf/base/handle.h>
#include "application_delegate.h"
#include "ipc.h"
#include "resource.h"

namespace ra::entry {
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


void SendCommandLineToOtherApplication(const std::wstring& command_line) {

    if (command_line.empty()) {
        return;
    }

    HWND ipc_window = FindWindow(ra::IPCWindowClassName, nullptr);
    if (!ipc_window) {
        return;
    }

    COPYDATASTRUCT copy_data_info{};
    copy_data_info.dwData = ra::IPCMessageIdentifier;
    copy_data_info.cbData = static_cast<DWORD>(command_line.length()) * sizeof(wchar_t);
    copy_data_info.lpData = const_cast<wchar_t*>(command_line.data());

    SendMessage(ipc_window, WM_COPYDATA, {}, reinterpret_cast<LPARAM>(&copy_data_info));
}

}

EntryResult RunGeneralEntry(const ApplicationSwithes& switches) {

    if (!CanApplicationRun()) {

        SendCommandLineToOtherApplication(switches.ToString());
        return EntryResult::AlreadyRunning;
    }

    zaf::InitializationOptions init_options;
    init_options.delegate = std::make_shared<ra::ApplicationDelegate>(switches);
    init_options.window_icon = LoadIcon(
        GetModuleHandle(nullptr),
        MAKEINTRESOURCE(IDI_APPICON));

    auto& application = zaf::Application::Instance();
    application.Initialize(init_options);
    application.Run();

    return EntryResult::OK;
}

}