#pragma once

#include <optional>
#include "module/calculator/parse/number_unit.h"

namespace ra::module::calculator {

class Modifier {
public:
    int base{ 10 };
    std::optional<int> bit_length;
    bool use_unsigned_type{};
    bool use_upper_case{};
    NumberUnit unit{ NumberUnit::Normal };
    std::optional<int> highlight_bit;
};

}