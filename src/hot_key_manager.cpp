#include "hot_key_manager.h"
#include <zaf/base/error/error.h>
#include <zaf/base/registry/registry.h>
#include "registry_define.h"

namespace ra {
namespace {

constexpr const wchar_t* const HotKeyValueName = L"HotKey";

constexpr int HotKeyID = 1;

}


HotKeyManager& HotKeyManager::Instance() {

    static HotKeyManager instance;
    return instance;
}


void HotKeyManager::Initialize() {

    ReadHotKeyFromRegistry();

    //Reset to default hot key if it is invalid.
    if (hot_key_.virtual_key == 0) {
        hot_key_.modifier = HotKeyModifier::Control;
        hot_key_.virtual_key = VK_SPACE;
    }

    RegisterHotKey();
}


void HotKeyManager::ReadHotKeyFromRegistry() {

    try {

        auto registry_key = zaf::Registry::CurrentUser().OpenSubKey(RegistrySubKeyPath);

        ULARGE_INTEGER large_integer{};
        large_integer.QuadPart = registry_key.GetQWordValue(HotKeyValueName);

        hot_key_.modifier = static_cast<HotKeyModifier>(large_integer.HighPart);
        hot_key_.virtual_key = large_integer.LowPart;
    }
    catch (const zaf::Error&) {

    }
}


void HotKeyManager::WriteHotKeyToRegistry() {

    try {

        auto registry_key = zaf::Registry::CurrentUser().CreateSubKey(
            RegistrySubKeyPath,
            zaf::RegistryRights::Write);

        ULARGE_INTEGER large_integer{};
        large_integer.HighPart = static_cast<DWORD>(hot_key_.modifier);
        large_integer.LowPart = hot_key_.virtual_key;

        registry_key.SetQWordValue(HotKeyValueName, large_integer.QuadPart);
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
                std::bind(&HotKeyManager::OnHotKeyMessage, this, std::placeholders::_1));
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


void HotKeyManager::OnHotKeyMessage(const zaf::Message& message) {

    if (message.id == WM_HOTKEY) {
        hot_key_pressed_subject_.GetObserver().OnNext({});
    }
}


void HotKeyManager::SetNewHotKey(const HotKey& new_hot_key) {

    UnregisterHotKey();

    hot_key_ = new_hot_key;

    WriteHotKeyToRegistry();
    RegisterHotKey();
}

}