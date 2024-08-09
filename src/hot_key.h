#pragma once

#include <Windows.h>
#include <cstdint>
#include <zaf/base/flags_enum.h>

namespace ra {

enum class HotKeyModifier {
    None = 0,
    Control = MOD_CONTROL,
    Alt = MOD_ALT,
    Shift = MOD_SHIFT,
    Win = MOD_WIN,
};

ZAF_ENABLE_FLAGS_ENUM(HotKeyModifier);

class HotKey {
public:
    HotKeyModifier modifier{ HotKeyModifier::None };
    std::uint32_t virtual_key{};
};

}