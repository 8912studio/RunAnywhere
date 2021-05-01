#pragma once

#include <optional>

namespace ra::module::calculator {

enum class NumberUnit {

    Normal,
    Kilo,
    Mega,
    Giga,
    Tera,
};

std::optional<NumberUnit> GetNumberUnitByChar(wchar_t ch);

}