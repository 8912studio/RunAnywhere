#pragma once

#include "utility/sql/orm/column.h"
#include "utility/sql/orm/column_value_traits.h"
#include "utility/sql/orm/composite_column.h"

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

private:
    T composite_column_;
};


template<typename T>
class Operand<T, std::enable_if_t<HasDataTypeMappingV<T>>> {
public:
    explicit Operand(T value) : value_(std::move(value)) {

    }

    std::string BuildSQL() const {
        return "?";
    }

    int BindParameters(Statement& statement, int begin_index) const {
        ColumnValueTraits<T>::BindValueToStatement(statement, begin_index, value_);
        return begin_index + 1;
    }

private:
    T value_{};
};


}
