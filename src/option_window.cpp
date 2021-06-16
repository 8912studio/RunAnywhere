#include "option_window.h"
#include <zaf/base/registry/registry.h>
#include <zaf/reflection/reflection_type_definition.h>
#include "hot_key_manager.h"

namespace ra {
namespace {

constexpr const wchar_t* const AutoRunRegistryPath =
L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

constexpr const wchar_t* const AutoRunValueName = L"RunAnywhere";

}

ZAF_DEFINE_REFLECTION_TYPE(OptionWindow)
ZAF_DEFINE_RESOURCE_URI(L"res:///option_window.xaml")
ZAF_DEFINE_END

std::weak_ptr<OptionWindow> g_instance;

void OptionWindow::ShowInstance() {

    auto instance = g_instance.lock();
    if (!instance) {
        instance = zaf::Create<OptionWindow>();
        g_instance = instance;
    }

    instance->Show();
    SetForegroundWindow(instance->GetHandle());
    BringWindowToTop(instance->GetHandle());
}


void OptionWindow::AfterParse() {

    __super::AfterParse();

    hotKeyBox->SetHotKey(HotKeyManager::Instance().GetCurrentHotKey());
    CheckIfHotKeyValid();

    CheckAutoRunCheckBoxState();

    Subscriptions() += hotKeyBox->HotKeyChangedEvent().Subscribe(
        std::bind(&OptionWindow::OnHotKeyChanged, this, std::placeholders::_1));

    Subscriptions() += autoRunCheckBox->CheckStateChangeEvent().Subscribe(
        std::bind(&OptionWindow::OnAutoRunCheckBoxStateChanged, this));
}


void OptionWindow::CheckIfHotKeyValid() {

    auto& hot_key_manager = HotKeyManager::Instance();
    auto hot_key = hot_key_manager.GetCurrentHotKey();

    bool should_show_label = (hot_key.virtual_key != 0) && !hot_key_manager.IsCurrentHotKeyValid();
    hotKeyInvalidLabel->SetIsVisible(should_show_label);
}


void OptionWindow::OnHotKeyChanged(const HotKey& hot_key) {

    HotKeyManager::Instance().SetNewHotKey(hot_key);
    CheckIfHotKeyValid();
}


void OptionWindow::CheckAutoRunCheckBoxState() {

    autoRunCheckBox->SetIsChecked(CheckIfSetAutoRun());
}


bool OptionWindow::CheckIfSetAutoRun() {

    try {

        auto key = zaf::Registry::CurrentUser().OpenSubKey(AutoRunRegistryPath);
        auto value = key.GetStringValue(AutoRunValueName);

        auto exe_file_path = zaf::Application::Instance().GetExeFilePath();
        return value.find(exe_file_path.wstring()) != 0;
    }
    catch (const zaf::Error&) {
        return false;
    }
}


void OptionWindow::OnAutoRunCheckBoxStateChanged() {

    SetAutoRunToRegistry(autoRunCheckBox->IsChecked());
    CheckAutoRunCheckBoxState();
}


void OptionWindow::SetAutoRunToRegistry(bool set) {

    try {

        auto key = zaf::Registry::CurrentUser().OpenSubKey(
            AutoRunRegistryPath,
            zaf::RegistryRights::Write);

        if (set) {

            auto exe_file_path = zaf::Application::Instance().GetExeFilePath();
            key.SetStringValue(AutoRunValueName, L'"' + exe_file_path.wstring() + L'"');
        }
        else {

            key.DeleteValue(AutoRunValueName);
        }
    }
    catch (const zaf::Error&) {

    }
}

}