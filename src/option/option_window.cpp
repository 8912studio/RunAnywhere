#include "option/option_window.h"
#include <zaf/application.h>
#include <zaf/base/registry/registry.h>
#include "entry/register_entry.h"
#include "hot_key_manager.h"
#include "option/option_storage.h"

namespace ra::option {
namespace {

constexpr const wchar_t* const AutoRunRegistryPath =
L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

constexpr const wchar_t* const AutoRunValueName = L"RunAnywhere";

std::wstring GetAutoRunPath() {
    auto exe_file_path = zaf::Application::Instance().GetExeFilePath();
    return L'"' + exe_file_path.wstring() + L'"';
}

}

ZAF_OBJECT_IMPL(OptionWindow);

std::weak_ptr<OptionWindow> g_instance;

void OptionWindow::ShowInstance() {

    auto instance = g_instance.lock();
    if (!instance) {
        instance = zaf::Create<OptionWindow>();
        g_instance = instance;
    }

    instance->Show();
    SetForegroundWindow(instance->Handle());
    BringWindowToTop(instance->Handle());
}


void OptionWindow::AfterParse() {

    __super::AfterParse();

    hotKeyBox->SetHotKey(HotKeyManager::Instance().GetCurrentHotKey());
    CheckIfHotKeyValid();

    UpdateAutoRunCheckBoxState();

    InitializeMaxPreservedCount();
    InitializeOpenAIAPIServer();
    InitializeOpenAIAPIKey();
    InitializeProxy();

    UpdateAutoHideCheckBoxState();
    UpdateRememberLastCommandCheckBoxState();

    Subscriptions() += hotKeyBox->HotKeyChangedEvent().Subscribe(
        std::bind(&OptionWindow::OnHotKeyChanged, this, std::placeholders::_1));

    Subscriptions() += autoRunCheckBox->CheckStateChangedEvent().Subscribe(
        std::bind(&OptionWindow::OnAutoRunCheckBoxStateChanged, this));

    Subscriptions() += autoHideCheckBox->CheckStateChangedEvent().Subscribe(
        std::bind(&OptionWindow::OnAutoHideCheckBoxStateChanged, this));

    Subscriptions() += rememberLastCommandCheckBox->CheckStateChangedEvent().Subscribe(
        std::bind(&OptionWindow::OnRememberLastCommandCheckBoxStateChanged, this));

    Subscriptions() += registerFileAssociationButton->ClickEvent().Subscribe(
        std::bind(&OptionWindow::OnRegisterFileAssociationButtonClick, this));
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


void OptionWindow::InitializeMaxPreservedCount() {

    maxPreservedCount->SetMinValue(OptionStorage::MinMaxPreservedCommandCount);
    maxPreservedCount->SetMaxValue(OptionStorage::MaxMaxPreservedCommandCount);
    maxPreservedCount->SetValue(
        static_cast<int>(OptionStorage::Instance().MaxPreservedCommandCount()));

    Subscriptions() += maxPreservedCount->ValueChangedEvent().Subscribe(std::bind([this]() {
        OptionStorage::Instance().SetMaxPreservedCommandCount(maxPreservedCount->Value());
    }));
}


void OptionWindow::InitializeOpenAIAPIServer() {

    apiServerEdit->SetText(OptionStorage::Instance().OpenAIAPIServer());

    Subscriptions() += apiServerEdit->ValueChangedEvent().Subscribe([](const std::wstring& value) {
        OptionStorage::Instance().SetOpenAIAPIServer(value);
    });
}


void OptionWindow::InitializeOpenAIAPIKey() {

    apiKeyEdit->SetText(OptionStorage::Instance().OpenAIAPIKey());

    Subscriptions() += apiKeyEdit->ValueChangedEvent().Subscribe([](const std::wstring& value) {
        OptionStorage::Instance().SetOpenAIAPIKey(value);
    });
}


void OptionWindow::InitializeProxy() {

    proxyEdit->SetText(OptionStorage::Instance().Proxy());

    Subscriptions() += proxyEdit->ValueChangedEvent().Subscribe([](const std::wstring& value) {
        OptionStorage::Instance().SetProxy(value);
    });
}


void OptionWindow::UpdateAutoRunCheckBoxState() {

    autoRunCheckBox->SetIsChecked(CheckIfSetAutoRun());
}


bool OptionWindow::CheckIfSetAutoRun() {

    try {

        auto key = zaf::Registry::CurrentUser().OpenSubKey(AutoRunRegistryPath);
        auto value = key.GetStringValue(AutoRunValueName);
        return value == GetAutoRunPath();
    }
    catch (const zaf::Error&) {
        return false;
    }
}


void OptionWindow::OnAutoRunCheckBoxStateChanged() {

    SetAutoRunToRegistry(autoRunCheckBox->IsChecked());
    UpdateAutoRunCheckBoxState();
}


void OptionWindow::SetAutoRunToRegistry(bool set) {

    try {

        auto key = zaf::Registry::CurrentUser().OpenSubKey(
            AutoRunRegistryPath,
            zaf::RegistryRights::Write);

        if (set) {
            key.SetStringValue(AutoRunValueName, GetAutoRunPath());
        }
        else {
            key.DeleteValue(AutoRunValueName);
        }
    }
    catch (const zaf::Error&) {

    }
}


void OptionWindow::UpdateAutoHideCheckBoxState() {

    autoHideCheckBox->SetIsChecked(OptionStorage::Instance().AutoHideOnLostFocus());
}


void OptionWindow::OnAutoHideCheckBoxStateChanged() {

    OptionStorage::Instance().SetAutoHideOnLostFocus(autoHideCheckBox->IsChecked());
}


void OptionWindow::UpdateRememberLastCommandCheckBoxState() {

    rememberLastCommandCheckBox->SetIsChecked(OptionStorage::Instance().RememberLastCommand());
}


void OptionWindow::OnRememberLastCommandCheckBoxStateChanged() {

    OptionStorage::Instance().SetRememberLastCommand(rememberLastCommandCheckBox->IsChecked());
}


void OptionWindow::OnRegisterFileAssociationButtonClick() {

    auto result = entry::RunRegisterEntry();

    std::wstring message;
    UINT box_type = MB_OK;

    if (result == entry::EntryResult::OK) {
        message = L"Register succeeded";
        box_type |= MB_ICONINFORMATION;
    }
    else {
        message = L"Register failed";
        box_type |= MB_ICONERROR;
    }

    MessageBox(this->Handle(), message.c_str(), L"Register file association", box_type);
}

}