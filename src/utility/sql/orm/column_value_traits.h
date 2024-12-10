#pragma once

#include <type_traits>
#include <zaf/base/type_traits/optional.h>
#include "utility/sql/data_type.h"
#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename T, typename = void>
struct MapToDataType;

template<typename T>
struct MapToDataType<T, std::enable_if_t<std::is_integral_v<T>>> {
    static constexpr DataType value = DataType::Integer;
};

template<>
struct MapToDataType<std::string> {
    static constexpr DataType value = DataType::Text;
};

template<typename T>
constexpr DataType MapToDataTypeV = MapToDataType<T>::value;


template<typename T, typename = void>
struct HasDataTypeMapping : std::false_type { };

template<typename T>
struct HasDataTypeMapping<T, std::void_t<decltype(MapToDataType<T>::value)>> : std::true_type {

};

template<typename T>
constexpr bool HasDataTypeMappingV = HasDataTypeMapping<T>::value;


template<typename T, typename = void>
struct ColumnValueTraits;

template<typename T>
struct ColumnValueTraits<T, 
    std::enable_if_t<zaf::IsOptionalV<T> && HasDataTypeMappingV<zaf::GetOptionalValueTypeT<T>>>> {

    static constexpr sql::DataType DataType = MapToDataTypeV<zaf::GetOptionalValueTypeT<T>>;
    static constexpr bool IsNullable = true;

    static void BindValueToStatement(Statement& statement, int parameter_index, const T& value) {
        if (value.has_value()) {
            statement.BindParameter(parameter_index, *value);
        }
    }
};

template<typename T>
struct ColumnValueTraits<T, std::enable_if_t<HasDataTypeMappingV<T>>> {

    static constexpr sql::DataType DataType = MapToDataTypeV<T>;
    static constexpr bool IsNullable = false;

    static void BindValueToStatement(Statement& statement, int parameter_index, const T& value) {
        statement.BindParameter(parameter_index, value);
    }
};


template<typename T, typename = void>
struct IsValidColumnValueType : std::false_type { };

template<typename T>
struct IsValidColumnValueType<T, std::void_t<decltype(ColumnValueTraits<T>::DataType)>> : 
    std::true_type { };

template<typename T>
constexpr bool IsValidColumnValueTypeV = IsValidColumnValueType<T>::value;

}