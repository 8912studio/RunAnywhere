#include "application_delegate.h"
#include <zaf/application.h>
#include <zaf/base/registry/registry.h>
#include <zaf/creation.h>
#include "about_window.h"
#include "environment_variable_manager.h"
#include "hot_key_manager.h"
#include "hot_key_utility.h"
#include "ipc.h"
#include "main_window.h"
#include "module/user_defined/bundle_definition.h"
#include "module/user_defined/import/import_bundle_window.h"
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

}


std::shared_ptr<ApplicationDelegate> ApplicationDelegate::GetFromApplication() {

    return std::dynamic_pointer_cast<ApplicationDelegate>(
        zaf::Application::Instance().GetDelegate());
}


ApplicationDelegate::ApplicationDelegate(const entry::ApplicationSwithes& switches) : 
    application_switches_(switches) {

}


void ApplicationDelegate::ReloadModules() {

    module_manager_->GetUserDefinedModule().Reload();
}


void ApplicationDelegate::ApplicationBeginRun(const zaf::ApplicationBeginRunInfo&) {

    EnvironmentVariableManager::Instance().Initialize();

    task_bar_create_message_id_ = RegisterWindowMessage(L"TaskbarCreated");

    InitializeTrayIconWindow();
    ShowTryIcon();
    InitializeHotKey();

    module_manager_ = std::make_shared<ModuleManager>();
    module_manager_->Initialize();

    main_window_ = zaf::Create<MainWindow>(module_manager_);

    RunApplicationSwitches(application_switches_);
}


void ApplicationDelegate::InitializeTrayIconWindow() {

    message_window_ = zaf::Create<zaf::Window>(IPCWindowClassName);
    message_window_->SetIsToolWindow(true);
    message_window_->SetActivateOption(zaf::ActivateOption::NoActivate);
    message_window_->SetInitialRectStyle(zaf::InitialRectStyle::Custom);
    message_window_->SetRect(zaf::Rect{});
    message_window_->CreateHandle();

    Subscriptions() += message_window_->MessageReceivedEvent().Subscribe(
        [this](const zaf::MessageReceivedInfo& event_info) {

        if (event_info.Message().ID() == WM_COPYDATA) {
            HandleIPCMessage(event_info.Message());
        }
        else if (event_info.Message().ID() == task_bar_create_message_id_) {
            ShowTryIcon();
        }
        else if (event_info.Message().ID() == WM_TRAY_ICON) {

            switch (event_info.Message().LParam()) {
            case WM_LBUTTONDBLCLK:
                main_window_->ShowOnTop();
                break;

            case WM_RBUTTONUP:
                PopupMenu();
                break;

            case NIN_BALLOONUSERCLICK:
                ra::OptionWindow::ShowInstance();
                break;
            }
        }
        else if (event_info.Message().ID() == WM_COMMAND && event_info.Message().LParam() == 0) {

            switch (LOWORD(event_info.Message().WParam())) {
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


void ApplicationDelegate::HandleIPCMessage(const zaf::Message& message) {

    auto copy_data_info = reinterpret_cast<const COPYDATASTRUCT*>(message.LParam());
    if (copy_data_info->dwData != IPCMessageIdentifier) {
        return;
    }

    std::wstring command_line_text(
        reinterpret_cast<const wchar_t*>(copy_data_info->lpData),
        copy_data_info->cbData / sizeof(wchar_t));

    entry::ApplicationSwithes switches{ command_line_text };
    RunApplicationSwitches(switches);
}


void ApplicationDelegate::RunApplicationSwitches(const entry::ApplicationSwithes& switches) {

    auto imported_path = switches.GetSwitchValue(entry::ImportSwitchName);
    if (!imported_path) {
        return;
    }

    std::filesystem::path bundle_path = *imported_path;
    if (bundle_path.extension() != module::user_defined::BundleFileExtension) {
        return;
    }

    auto importer = module_manager_->GetUserDefinedModule().BeginImportBundle(bundle_path);
    auto import_window = zaf::Create<module::user_defined::ImportBundleWindow>(importer);
    import_window->Show();
    SetForegroundWindow(import_window->Handle());
}


void ApplicationDelegate::ShowTryIcon() {

    ra::AddTrayIcon(message_window_->Handle(), WM_TRAY_ICON);
}


void ApplicationDelegate::PopupMenu() {

    if (!menu_) {
        menu_ = LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDR_APPMENU));
        menu_ = GetSubMenu(menu_, 0);
    }

    POINT current_position{};
    GetCursorPos(&current_position);

    SetForegroundWindow(message_window_->Handle());

    TrackPopupMenu(
        menu_,
        TPM_RIGHTBUTTON | TPM_NOANIMATION,
        current_position.x,
        current_position.y,
        0,
        message_window_->Handle(),
        nullptr);
}


void ApplicationDelegate::InitializeHotKey() {

    auto& hot_key_manager = ra::HotKeyManager::Instance();
    hot_key_manager.Initialize();

    Subscriptions() += hot_key_manager.HotKeyPressedEvent().Subscribe([this](zaf::None) {

        if (main_window_->IsVisible() && main_window_->IsFocused()) {
            main_window_->Hide();
        }
        else {
            main_window_->ShowOnTop();
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


void ApplicationDelegate::ApplicationEndRun(const zaf::ApplicationEndRunInfo&) {

    main_window_->Close();
    main_window_.reset();

	ra::RemoveTrayIcon();
}


void ApplicationDelegate::SessionEnded(const zaf::SessionEndedInfo&) {

	zaf::Application::Instance().Terminate();
}

}