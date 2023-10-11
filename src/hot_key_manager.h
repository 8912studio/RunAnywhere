#pragma once

#include <zaf/base/none.h>
#include <zaf/window/message_only_window.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/subscription_host.h>
#include "hot_key.h"

namespace ra {

class HotKeyManager : public zaf::SubscriptionHost {
public:
    static HotKeyManager& Instance();

public:
    HotKeyManager(const HotKeyManager&) = delete;
    HotKeyManager& operator=(const HotKeyManager&) = delete;

    void Initialize();

    void SetNewHotKey(const HotKey& new_hot_key);

    const HotKey& GetCurrentHotKey() const {
        return hot_key_;
    }

    bool IsCurrentHotKeyValid() const {
        return is_hot_key_valid_;
    }

    zaf::Observable<zaf::None> HotKeyPressedEvent() {
        return hot_key_pressed_subject_.AsObservable();
    }

private:
    HotKeyManager() = default;

    void ReadHotKeyFromRegistry();
    void WriteHotKeyToRegistry();
    void RegisterHotKey();
    void UnregisterHotKey();
    void OnHotKeyMessage(const zaf::Message& message);

private:
    HotKey hot_key_;
    bool is_hot_key_valid_{};

    std::unique_ptr<zaf::MessageOnlyWindow> hot_key_message_window_;
    zaf::Subject<zaf::None> hot_key_pressed_subject_;
};

}