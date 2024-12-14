#pragma once

#include "utility/sql/orm/value_type/data_type_mapping.h"
#include "utility/sql/orm/value_type/value_type_traits.h"
#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename T, typename = void>
struct IsPrimitiveValueType : std::false_type {};

template<typename T>
struct IsPrimitiveValueType<T, std::void_t<decltype(MapToDataType<T>::value)>> : std::true_type {};

template<typename T>
constexpr bool IsPrimitiveValueTypeV = IsPrimitiveValueType<T>::value;


template<typename T>
struct ValueTypeTraits<T, std::enable_if_t<IsPrimitiveValueTypeV<T>>> {

    static constexpr sql::DataType DataType = MapToDataTypeV<T>;
    static constexpr bool IsNullable = false;

    static constexpr std::size_t PlaceholderCount = 1;

    static int BindValueToStatement(Statement& statement, int parameter_index, const T& value) {
        statement.BindParameter(parameter_index, value);
        return parameter_index + 1;
    }

    static T GetValueFromStatement(const Statement& statement, int column_index) {

        if constexpr (std::is_integral_v<T>) {
            if constexpr (sizeof(T) >= sizeof(std::int64_t)) {
                return static_cast<T>(statement.GetColumnInt64(column_index));
            }
            return static_cast<T>(statement.GetColumnInt(column_index));
        }

        if constexpr (std::is_same_v<std::string, T>) {
            return std::string{ statement.GetColumnText(column_index) };
        }
    }
};

}