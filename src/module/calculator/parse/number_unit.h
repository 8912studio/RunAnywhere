#pragma once

#include <optional>

namespace ra::mod::calculator {

enum class NumberUnit {

    Normal,
    Kilo,
    Mega,
    Giga,
    Tera,
};

std::optional<NumberUnit> GetNumberUnitByChar(wchar_t ch);

}