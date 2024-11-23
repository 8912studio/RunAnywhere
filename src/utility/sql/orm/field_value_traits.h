#pragma once

#include <type_traits>
#include "utility/sql/data_type.h"

namespace ra::utility::sql {

template<typename T, typename = void>
struct FieldValueTraits { };

template<typename T>
struct FieldValueTraits<T, std::enable_if_t<std::is_integral_v<T>>> {
    static constexpr sql::DataType DataType = sql::DataType::Integer;
};

template<>
struct FieldValueTraits<std::string> {
    static constexpr sql::DataType DataType = sql::DataType::Text;
};

}