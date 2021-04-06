#include "module/calculator/parse/number_unit.h"
#include <cctype>

namespace calculator {

std::optional<NumberUnit> GetNumberUnitByChar(wchar_t ch) {

    static constexpr struct MapItem {
        wchar_t ch;
        NumberUnit unit;
    } map[] = {
        { L'k', NumberUnit::Kilo },
        { L'm', NumberUnit::Mega },
        { L'g', NumberUnit::Giga },
        { L't', NumberUnit::Tera },
    };

    auto lower_char = std::tolower(ch);
    for (const auto& each_item : map) {

        if (lower_char == each_item.ch) {
            return each_item.unit;
        }
    }

    return std::nullopt;
}

}