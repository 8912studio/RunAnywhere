#pragma once

#include <Windows.h>
#include <zaf/object/enum_declaration.h>

namespace ra::mod::user_defined {

enum class ShowWindowOption {
    Normal = SW_NORMAL,
    Hide = SW_HIDE,
};

ZAF_DECLARE_ENUM(ShowWindowOption);

}

ZAF_ENABLE_ENUM_BOXING(ra::mod::user_defined::ShowWindowOption);