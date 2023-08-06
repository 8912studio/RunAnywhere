#pragma once

#include <optional>
#include <string_view>

namespace ra::mod::tool::date {

enum class DateTimeUnit {
    Second,
    Minute,
    Hour,
    Day,
    Week,
    Month,
    Year,
};

std::optional<DateTimeUnit> DateTimeUnitFromString(std::wstring_view string);

}