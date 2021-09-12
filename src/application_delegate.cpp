#include "application_delegate.h"
#include <zaf/application.h>
#include <zaf/base/registry/registry.h>
#include <zaf/creation.h>
#include "about_window.h"
#include "hot_key_manager.h"
#include "hot_key_utility.h"
#include "main_window.h"
#include "option_window.h"
#include "registry_define.h"
#include "resource.h"
#include "tray_icon.h"

namespace ra {
namespace {

constexpr UINT WM_TRAY_ICON = WM_USER + 1;

constexpr const wchar_t* HasShownWelcomeNotificationValueName = L"HasShownWelcomeNotification";


bool HasShownWelcomeNotification() {

    try {

        auto register_key = zaf::Registry::CurrentUser().OpenSubKey(ra::RegistrySubKeyPath);

        auto value = register_key.GetDWordValue(HasShownWelcomeNotificationValueName);
        return !!value;
    }
    catch (const zaf::Error&) {
        return false;
    }
}


void SetHasShownWelcomeNotification() {

    try {

        auto register_key = zaf::Registry::CurrentUser().CreateSubKey(
            ra::RegistrySubKeyPath,
            zaf::RegistryRights::Write);

        register_key.SetDWordValue(HasShownWelcomeNotificationValueName, 1);
    }
    catch (const zaf::Error&) {

    }
}


void ShowWelcomeNotification() {

    if (HasShownWelcomeNotification()) {
        return;
    }

    std::wstring info{ L"Press \"" };
    info += ra::GenerateTextByHotKey(ra::HotKeyManager::Instance().GetCurrentHotKey());
    info += L"\" to call out";
    ra::ShowBalloon(L"RunAnywhere is running", info.c_str());

    SetHasShownWelcomeNotification();
}


void InitializeHotKey() {

    auto& hot_key_manager = ra::HotKeyManager::Instance();
    hot_key_manager.Initialize();

    zaf::Application::Instance().Subscriptions() += hot_key_manager.HotKeyPressedEvent().Subscribe(
        [](zaf::Dumb) {

        auto& main_window = ra::MainWindow::Instance();

        if (IsWindowVisible(main_window.Handle())) {
            main_window.Hide();
        }
        else {
            main_window.ShowOnTop();
        }
    });

    //Show a notification tip if hot key is invalid.
    if (!hot_key_manager.IsCurrentHotKeyValid()) {

        std::wstring title{ L"Fail to register \"" };
        title += ra::GenerateTextByHotKey(hot_key_manager.GetCurrentHotKey());
        title += L'"';

        ra::ShowBalloon(title, L"Please check the hot key setting");
    }
    //Show a welcome notification tip.
    else {

        ShowWelcomeNotification();
    }
}

}


void ApplicationDelegate::ApplicationBeginRun(const zaf::ApplicationBeginRunInfo&) {

    task_bar_create_message_id_ = RegisterWindowMessage(L"TaskbarCreated");

    InitializeTrayIconWindow();
	ShowTryIcon();
	InitializeHotKey();

	//Access instance to create main window object.
	ra::MainWindow::Instance();
}


void ApplicationDelegate::InitializeTrayIconWindow() {

    tray_icon_window_ = zaf::Create<zaf::Window>();
    tray_icon_window_->SetIsToolWindow(true);
    tray_icon_window_->SetActivateOption(zaf::ActivateOption::NoActivate);
    tray_icon_window_->SetInitialRectStyle(zaf::InitialRectStyle::Custom);
    tray_icon_window_->SetRect(zaf::Rect{});
    tray_icon_window_->CreateHandle();

    Subscriptions() += tray_icon_window_->ReceiveMessageEvent().Subscribe(
        [this](const zaf::WindowReceiveMessageInfo& event_info) {
    
        if (event_info.message.id == task_bar_create_message_id_) {
            ShowTryIcon();
        }
        else if (event_info.message.id == WM_TRAY_ICON) {

            switch (event_info.message.lparam) {
            case WM_LBUTTONDBLCLK:
                ra::MainWindow::Instance().ShowOnTop();
                break;

            case WM_RBUTTONUP:
                PopupMenu();
                break;

            case NIN_BALLOONUSERCLICK:
                ra::OptionWindow::ShowInstance();
                break;
            }
        }
        else if (event_info.message.id == WM_COMMAND && event_info.message.lparam == 0) {

            switch (LOWORD(event_info.message.wparam)) {
            case ID_TRAYICON_EXIT:
                zaf::Application::Instance().Terminate();
                break;

            case ID_TRAYICON_OPTION:
                ra::OptionWindow::ShowInstance();
                break;

            case ID_TRAYICON_ABOUT:
                ra::AboutWindow::ShowInstance();
                break;
            }
        }
    });
}


void ApplicationDelegate::ShowTryIcon() {

    ra::AddTrayIcon(tray_icon_window_->Handle(), WM_TRAY_ICON);
}


void ApplicationDelegate::PopupMenu() {

    if (!menu_) {
        menu_ = LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDR_APPMENU));
        menu_ = GetSubMenu(menu_, 0);
    }

    POINT current_position{};
    GetCursorPos(&current_position);

    SetForegroundWindow(tray_icon_window_->Handle());

    TrackPopupMenu(
        menu_,
        TPM_RIGHTBUTTON | TPM_NOANIMATION,
        current_position.x,
        current_position.y,
        0,
        tray_icon_window_->Handle(),
        nullptr);
}


void ApplicationDelegate::ApplicationEndRun(const zaf::ApplicationEndRunInfo&) {

	ra::MainWindow::Instance().Close();

	ra::RemoveTrayIcon();
}


void ApplicationDelegate::SessionEnded(const zaf::SessionEndedInfo&) {

	zaf::Application::Instance().Terminate();
}

}