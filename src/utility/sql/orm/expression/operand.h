#pragma once

#include "utility/sql/orm/column.h"
#include "utility/sql/orm/composite_column.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/value_type/composite_value_type.h"

namespace ra::utility::sql {

template<typename T, typename = void>
class Operand;


template<typename T>
class Operand<T, std::enable_if_t<IsColumnV<T>>> {
public:
    explicit Operand(const T& column) : column_(&column) {

    }

    std::string BuildSQL() const {
        return std::string{ column_->GetName() };
    };

    int BindParameters(Statement& statement, int begin_index) const {
        return begin_index;
    }

private:
    const T* column_{};
};


template<typename T>
class Operand<T, std::enable_if_t<IsCompositeColumnV<T>>> {
public:
    explicit Operand(const T& composite_column) : composite_column_(composite_column) {

    }

    std::string BuildSQL() const {
        return std::format("({})", JoinColumnNames(composite_column_.GetAbstractColumns()));
    }

    int BindParameters(Statement& statement, int begin_index) const {
        return begin_index;
    }

private:
    T composite_column_;
};


template<typename T>
class Operand<T, std::enable_if_t<
    IsPrimitiveValueTypeV<T> || IsNullableValueTypeV<T> || IsCompositeValueTypeV<T>>> {

public:
    explicit Operand(T value) : value_(std::move(value)) {

    }

    std::string BuildSQL() const {
        constexpr auto place_holder_count = ValueTypeTraits<T>::PlaceholderCount;
        if constexpr (place_holder_count == 1) {
            return "?";
        }
        return std::format("({})", JoinPlaceholders(place_holder_count));
    }

    int BindParameters(Statement& statement, int begin_index) const {
        return ValueTypeTraits<T>::BindValueToStatement(statement, begin_index, value_);
    }

private:
    T value_{};
};

}
