#include "hot_key_box.h"
#include <zaf/reflection/reflection_type_definition.h>
#include <zaf/window/message/keyboard_message.h>

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


bool IsFunctionalKey(UINT virtual_key) {
    return (VK_F1 <= virtual_key && virtual_key <= VK_F12);
}


bool IsDependentKey(UINT virtual_key) {

    auto ch = MapVirtualKey(virtual_key, MAPVK_VK_TO_CHAR);
    if (ch != 0) {
        return true;
    }

    switch (virtual_key) {
    case VK_TAB:
    case VK_SPACE:
    case VK_INSERT:
    case VK_DELETE:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_LEFT:
    case VK_UP:
    case VK_RIGHT:
    case VK_DOWN:
        return true;
    default:
        return false;
    }
}


bool IsIndependentKey(UINT virtual_key) {
    return IsFunctionalKey(virtual_key);
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


std::wstring GetKeyText(UINT virtual_key) {

    auto ch = LOWORD(MapVirtualKey(virtual_key, MAPVK_VK_TO_CHAR));
    if (ch != 0) {
        return std::wstring(1, static_cast<wchar_t>(ch));
    }

    if (IsFunctionalKey(virtual_key)) {
        return L"F" + std::to_wstring(virtual_key - VK_F1 + 1);
    }

    static constexpr struct {
        UINT virtual_key;
        const wchar_t* text;
    } key_text_map[] = {
    
        { VK_TAB, L"Tab" },
        { VK_SPACE, L"Space" },
        { VK_INSERT, L"Insert" },
        { VK_DELETE, L"Delete" },
        { VK_HOME, L"Home" },
        { VK_END, L"End" },
        { VK_PRIOR, L"PageUp" },
        { VK_NEXT, L"PageDown" },
        { VK_LEFT, L"Left" },
        { VK_UP, L"Up" },
        { VK_RIGHT, L"Right" },
        { VK_DOWN, L"Down" },
    };

    for (const auto& each_item : key_text_map) {

        if (virtual_key == each_item.virtual_key) {
            return each_item.text;
        }
    }

    return std::wstring{};
}


std::wstring GenerateTextByHotKey(const HotKey& hot_key) {

    if (hot_key.virtual_key == 0) {
        return L"None";
    }

    std::wstring result;

    if ((hot_key.modifier & HotKeyModifier::Control) == HotKeyModifier::Control) {
        result += L"Ctrl + ";
    }

    if ((hot_key.modifier & HotKeyModifier::Shift) == HotKeyModifier::Shift) {
        result += L"Shift + ";
    }

    if ((hot_key.modifier & HotKeyModifier::Alt) == HotKeyModifier::Alt) {
        result += L"Alt + ";
    }

    if ((hot_key.modifier & HotKeyModifier::Win) == HotKeyModifier::Win) {
        result += L"Win + ";
    }

    result += GetKeyText(hot_key.virtual_key);
    return result;
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
