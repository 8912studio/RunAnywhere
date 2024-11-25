#pragma once

#include <type_traits>
#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename T>
T GetValueFromStatement(const Statement& statement, int column_index) {

    if constexpr (std::is_integral_v<T>) {
        if constexpr (sizeof(T) >= sizeof(std::int64_t)) {
            return static_cast<T>(statement.GetColumnInt64(column_index));
        }
        return static_cast<T>(statement.GetColumnInt(column_index));
    }

    if constexpr (std::is_same_v<std::string, T>) {
        return std::string{ statement.GetColumnText(column_index) };
    }

    return {};
}

}