#pragma once

#include <zaf/base/type_traits/optional.h>
#include "utility/sql/orm/value_type/primitive_value_type.h"

namespace ra::utility::sql {

template<typename T>
constexpr bool IsNullableValueTypeV =
    zaf::IsOptionalV<T> &&
    IsPrimitiveValueTypeV<zaf::GetOptionalValueTypeT<T>>;


template<typename T>
struct ValueTypeTraits<T, std::enable_if_t<IsNullableValueTypeV<T>>> {

    static constexpr std::size_t PlaceholderCount = 1;

    static int BindValueToStatement(Statement& statement, int parameter_index, const T& value) {
        if (value.has_value()) {
            statement.BindParameter(parameter_index, *value);
        }
        return parameter_index + 1;
    }

    static T GetValueFromStatement(const Statement& statement, int column_index) {

        auto column_type = statement.GetColumnType(column_index);
        if (column_type == DataType::Null) {
            return std::nullopt;
        }

        return ValueTypeTraits<zaf::GetOptionalValueTypeT<T>>::GetValueFromStatement(
            statement,
            column_index);
    }
};

}