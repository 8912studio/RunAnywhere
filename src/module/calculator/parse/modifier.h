#pragma once

#include <optional>
#include "module/calculator/parse/number_unit.h"

namespace calculator {

class Modifier {
public:
    int base{ 10 };
    int bit_length{ 32 };
    bool use_upper_case{};
    NumberUnit unit{ NumberUnit::Normal };
    std::optional<int> highlight_bit;
};

}