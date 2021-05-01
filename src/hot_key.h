#pragma once

#include <Windows.h>
#include <cstdint>
#include <zaf/base/flag_enum.h>

namespace ra {

enum class HotKeyModifier {
    None = 0,
    Control = MOD_CONTROL,
    Alt = MOD_ALT,
    Shift = MOD_SHIFT,
    Win = MOD_WIN,
};

ZAF_ENABLE_FLAG_ENUM(HotKeyModifier);

class HotKey {
public:
    HotKeyModifier modifier{ HotKeyModifier::None };
    std::uint32_t virtual_key{};
};

}