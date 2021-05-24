#include "hot_key_utility.h"

namespace ra {
namespace {

std::wstring GetKeyText(UINT virtual_key) {

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

    auto ch = LOWORD(MapVirtualKey(virtual_key, MAPVK_VK_TO_CHAR));
    if (ch != 0) {
        return std::wstring(1, static_cast<wchar_t>(ch));
    }

    if (IsFunctionalKey(virtual_key)) {
        return L"F" + std::to_wstring(virtual_key - VK_F1 + 1);
    }

    return std::wstring{};
}

}


bool IsFunctionalKey(UINT virtual_key) {
    return (VK_F1 <= virtual_key && virtual_key <= VK_F12);
}


bool IsIndependentKey(UINT virtual_key) {
    return IsFunctionalKey(virtual_key);
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