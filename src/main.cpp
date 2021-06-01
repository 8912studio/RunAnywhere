#include <zaf/application.h>
#include <zaf/base/handle.h>
#include "application_delegate.h"
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

}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    if (!CanApplicationRun()) {
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