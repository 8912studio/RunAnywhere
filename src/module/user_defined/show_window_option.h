#pragma once

#include <Windows.h>
#include <zaf/object/enum_support.h>

namespace ra::mod::user_defined {

enum class ShowWindowOption {
    Normal = SW_NORMAL,
    Hide = SW_HIDE,
};

ZAF_ENUM_BEGIN(ShowWindowOption);
ZAF_ENUM_CONSTANT(Normal);
ZAF_ENUM_CONSTANT(Hide);
ZAF_ENUM_END;

}