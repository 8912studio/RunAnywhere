#include "hot_key_manager.h"
#include <zaf/base/error/error.h>
#include <zaf/base/registry/registry.h>

namespace {

constexpr const wchar_t* const RegistrySubKeyPath = L"Software\\RunAnywhere";
constexpr const wchar_t* const ModifierValueName = L"HotKeyModifier";
constexpr const wchar_t* const VirtualKeyValueName = L"HotKeyVirtualKey";

constexpr int HotKeyID = 1;

}


HotKeyManager& HotKeyManager::Instance() {

    static HotKeyManager instance;
    return instance;
}


void HotKeyManager::Initialize() {

    ReadHotKeyFromRegistry();
    RegisterHotKey();
}


void HotKeyManager::ReadHotKeyFromRegistry() {

    try {

        auto registry_key = zaf::Registry::CurrentUser().OpenSubKey(RegistrySubKeyPath);

        hot_key_.modifier = 
            static_cast<HotKeyModifier>(registry_key.GetDWordValue(ModifierValueName));
        hot_key_.virtual_key = registry_key.GetDWordValue(VirtualKeyValueName);
    }
    catch (const zaf::Error&) {

        hot_key_.modifier = HotKeyModifier::None;
        hot_key_.virtual_key = 0;
    }
}


void HotKeyManager::WriteHotKeyToRegistry() {

    try {

        auto registry_key = zaf::Registry::CurrentUser().CreateSubKey(
            RegistrySubKeyPath,
            zaf::RegistryRights::Write);

        registry_key.SetDWordValue(ModifierValueName, static_cast<DWORD>(hot_key_.modifier));
        registry_key.SetDWordValue(VirtualKeyValueName, hot_key_.virtual_key);
    }
    catch (const zaf::Error&) {

    }
}


void HotKeyManager::RegisterHotKey() {

    if (hot_key_.virtual_key == 0) {
        return;
    }

    if (!hot_key_message_window_) {

        hot_key_message_window_ = std::make_unique<zaf::MessageOnlyWindow>();

        Subscriptions() +=
            hot_key_message_window_->ReceiveMessageEvent().Subscribe(
                std::bind(&HotKeyManager::OnHotKeyPressed, this));
    }

    BOOL is_succeeded = ::RegisterHotKey(
        hot_key_message_window_->GetHandle(),
        HotKeyID,
        static_cast<UINT>(hot_key_.modifier),
        hot_key_.virtual_key);

    is_hot_key_valid_ = !!is_succeeded;
}


void HotKeyManager::UnregisterHotKey() {

    if (!hot_key_message_window_) {
        return;
    }

    ::UnregisterHotKey(hot_key_message_window_->GetHandle(), HotKeyID);
    is_hot_key_valid_ = false;
}


void HotKeyManager::OnHotKeyPressed() {

    hot_key_pressed_subject_.GetObserver().OnNext({});
}


void HotKeyManager::SetNewHotKey(const HotKey& new_hot_key) {

    UnregisterHotKey();

    hot_key_ = new_hot_key;

    WriteHotKeyToRegistry();
    RegisterHotKey();
}
