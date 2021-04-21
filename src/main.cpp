#include <zaf/application.h>
#include <zaf/base/error/system_error.h>
#include <zaf/base/handle.h>
#include <zaf/creation.h>
#include <zaf/window/message_only_window.h>
#include "hot_key_manager.h"
#include "main_window.h"
#include "option_window.h"
#include "resource.h"
#include "tray_icon.h"

namespace {

zaf::Handle g_exclusive_mutex;
std::unique_ptr<zaf::MessageOnlyWindow> g_tray_icon_message_window;
HMENU g_menu{};

constexpr UINT WM_TRAY_ICON = WM_USER + 1;

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


void PopupMenu() {

    if (!g_menu) {
        g_menu = LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDR_MENU1));
        g_menu = GetSubMenu(g_menu, 0);
    }

    POINT current_position{};
    GetCursorPos(&current_position);

    SetForegroundWindow(g_tray_icon_message_window->GetHandle());

    TrackPopupMenu(
        g_menu,
        TPM_RIGHTBUTTON | TPM_NOANIMATION,
        current_position.x,
        current_position.y,
        0,
        g_tray_icon_message_window->GetHandle(),
        nullptr);
}


void ShowTryIcon() {

    g_tray_icon_message_window = std::make_unique<zaf::MessageOnlyWindow>();
    zaf::Application::Instance().Subscriptions() +=
        g_tray_icon_message_window->ReceiveMessageEvent().Subscribe([](const zaf::Message& message) {

        if (message.id == WM_TRAY_ICON) {

            switch (message.lparam) {
            case WM_LBUTTONDBLCLK:
                MainWindow::Instance().ShowOnTop();
                break;

            case WM_RBUTTONUP:
                PopupMenu();
                break;
            }
        }
        else if (message.id == WM_COMMAND && message.lparam == 0) {

            switch (LOWORD(message.wparam)) {
            case ID_TRAYICON_EXIT:
                zaf::Application::Instance().Terminate();
                break;

            case ID_TRAYICON_OPTION:
                OptionWindow::ShowInstance();
                break;
            }
        }
    });

    AddTrayIcon(g_tray_icon_message_window->GetHandle(), WM_TRAY_ICON);
}


void InitializeHotKey() {

    auto& hot_key_manager = HotKeyManager::Instance();
    hot_key_manager.Initialize();

    zaf::Application::Instance().Subscriptions() += hot_key_manager.HotKeyPressedEvent().Subscribe(
        [](zaf::Dumb) {
    
        auto& main_window = MainWindow::Instance();

        if (IsWindowVisible(main_window.GetHandle())) {
            main_window.Hide();
        }
        else {
            main_window.ShowOnTop();
        }
    });
}


void BeginRun(const zaf::ApplicationBeginRunInfo&) {

    ShowTryIcon();
    InitializeHotKey();

    //Access instance to create main window object.
    auto& main_window = MainWindow::Instance();

#ifndef NDEBUG
    main_window.Show();
#endif
}


void EndRun(const zaf::ApplicationEndRunInfo&) {

    MainWindow::Instance().Close();

    RemoveTrayIcon();
}

}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    if (!CanApplicationRun()) {
        return 0;
    }

    auto& application = zaf::Application::Instance();
    application.Subscriptions() += application.BeginRunEvent().Subscribe(BeginRun);
    application.Subscriptions() += application.EndRunEvent().Subscribe(EndRun);

    application.Initialize({});
    application.Run();
    return 0;
}