#pragma once

#include <Windows.h>
#include <zaf/object/enum_declaration.h>

namespace ra::module::user_defined {

enum class ShowWindowOption {
    Normal = SW_NORMAL,
    Hide = SW_HIDE,
};

ZAF_DECLARE_ENUM(ShowWindowOption);

}