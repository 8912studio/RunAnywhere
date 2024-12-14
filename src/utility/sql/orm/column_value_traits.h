#pragma once

#include <type_traits>
#include <zaf/base/type_traits/optional.h>
#include "utility/sql/data_type.h"
#include "utility/sql/statement.h"
#include "utility/sql/orm/value_type/data_type_mapping.h"
#include "utility/sql/orm/value_type/nullable_value_type.h"
#include "utility/sql/orm/value_type/primitive_value_type.h"

namespace ra::utility::sql {

template<typename T, typename = void>
struct ColumnValueTraits;

template<typename T>
struct ColumnValueTraits<T, std::enable_if_t<IsPrimitiveValueTypeV<T>>> {

    static constexpr sql::DataType DataType = MapToDataTypeV<T>;
    static constexpr bool IsNullable = false;

    static void BindValueToStatement(Statement& statement, int parameter_index, const T& value) {
        ValueTypeTraits<T>::BindValueToStatement(statement, parameter_index, value);
    }
};

template<typename T>
struct ColumnValueTraits<T, std::enable_if_t<IsNullableValueTypeV<T>>> {

    static constexpr sql::DataType DataType = MapToDataTypeV<zaf::GetOptionalValueTypeT<T>>;
    static constexpr bool IsNullable = true;

    static void BindValueToStatement(Statement& statement, int parameter_index, const T& value) {
        ValueTypeTraits<T>::BindValueToStatement(statement, parameter_index, value);
    }
};

}