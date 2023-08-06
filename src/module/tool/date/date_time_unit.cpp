#include "module/tool/date/date_time_unit.h"

namespace ra::mod::tool::date {

std::optional<DateTimeUnit> DateTimeUnitFromString(std::wstring_view string) {

    if (string == L"s") {
        return DateTimeUnit::Second;
    }
    if (string == L"m" || string == L"mi" || string == L"min") {
        return DateTimeUnit::Minute;
    }
    if (string == L"h") {
        return DateTimeUnit::Hour;
    }
    if (string == L"d") {
        return DateTimeUnit::Day;
    }
    if (string == L"w") {
        return DateTimeUnit::Week;
    }
    if (string == L"mo" || string == L"mon") {
        return DateTimeUnit::Month;
    }
    if (string == L"y") {
        return DateTimeUnit::Year;
    }
    return std::nullopt;
}

}