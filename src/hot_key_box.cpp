#include "hot_key_box.h"
#include <zaf/reflection/reflection_type_definition.h>
#include <zaf/window/message/keyboard_message.h>
#include "hot_key_utility.h"

namespace ra {
namespace {

HotKeyModifier GetModifier() {

    static constexpr struct {
        UINT virtual_key;
        HotKeyModifier modifier;
    } modifier_map[] = {

        { VK_CONTROL, HotKeyModifier::Control },
        { VK_SHIFT, HotKeyModifier::Shift },
        { VK_MENU, HotKeyModifier::Alt },
        { VK_LWIN, HotKeyModifier::Win },
        { VK_RWIN, HotKeyModifier::Win },
    };

    HotKeyModifier result{ HotKeyModifier::None };

    for (const auto& each_item : modifier_map) {

        SHORT key_state = GetKeyState(each_item.virtual_key);
        if ((key_state >> 15) != 0) {

            result |= each_item.modifier;
        }
    }

    return result;
}


std::optional<HotKey> GenerateHotKeyFromKeyMessage(const zaf::KeyMessage& message) {

    auto key = message.GetVirtualKey();
    if (key == VK_BACK) {
        return HotKey{};
    }

    HotKeyModifier modifier = GetModifier();

    if (IsDependentKey(key)) {

        if (modifier == HotKeyModifier::None) {
            return std::nullopt;
        }

        HotKey hot_key;
        hot_key.modifier = modifier;
        hot_key.virtual_key = key;
        return hot_key;
    }

    if (IsIndependentKey(key)) {

        HotKey hot_key;
        hot_key.modifier = modifier;
        hot_key.virtual_key = key;
        return hot_key;
    }

    return std::nullopt;
}

}


ZAF_DEFINE_REFLECTION_TYPE(HotKeyBox)
ZAF_DEFINE_END


void HotKeyBox::Initialize() {

    __super::Initialize();

    SetPadding(zaf::Frame{ 8, 0, 8, 0 });
    SetBorder(1);
    SetBorderColorPicker([this](const zaf::Control&) {

        if (is_waiting_input_) {
            return zaf::Color::FromRGB(0x0587FF);
        }

        return zaf::Color::Black();
    });

    SetBackgroundColor(zaf::Color::White());

    UpdateText();
}


void HotKeyBox::SetHotKey(const HotKey& hot_key) {

    hot_key_ = hot_key;
    UpdateText();

    hot_key_changed_subject_.GetObserver().OnNext(hot_key_);
}


bool HotKeyBox::AcceptKeyMessage(const zaf::KeyMessage& message) {

    if (message.id == WM_KEYDOWN && message.GetVirtualKey() == VK_ESCAPE) {

        if (is_waiting_input_) {
            return true;
        }
    }

    return false;
}


bool HotKeyBox::OnKeyDown(const zaf::KeyMessage& message) {

    if (!is_waiting_input_) {
        return true;
    }

    //ESC to cancel setting hot key.
    if (message.GetVirtualKey() == VK_ESCAPE) {
        SetIsWaitingInput(false);
        return true;
    }

    auto new_hot_key = GenerateHotKeyFromKeyMessage(message);
    if (!new_hot_key) {
        return true;
    }

    SetHotKey(*new_hot_key);
    SetIsWaitingInput(false);
    return true;
}


void HotKeyBox::OnFocusLose() {

    __super::OnFocusLose();

    SetIsWaitingInput(false);
}


void HotKeyBox::OnClick() {

    SetIsWaitingInput(!is_waiting_input_);
}


void HotKeyBox::SetIsWaitingInput(bool value) {

    is_waiting_input_ = value;
    UpdateText();
}


void HotKeyBox::UpdateText() {

    std::wstring text;
    if (is_waiting_input_) {
        text = L"Waiting input...";
    }
    else {
        text = GenerateTextByHotKey(hot_key_);
    }
    this->SetText(text);
}

}