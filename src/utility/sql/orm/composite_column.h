#pragma once

#include <array>
#include "utility/sql/orm/column.h"
#include "utility/sql/orm/value_type/composite_value_type.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class BaseCompositeColumn {
public:
    explicit BaseCompositeColumn(const Columns&... columns) : columns_(&columns...) {

    }

    AbstractColumnsView GetAbstractColumns() const noexcept {
        return {
            reinterpret_cast<const AbstractColumn* const*>(columns_.data()),
            columns_.size()
        };
    }

    ColumnsView<E> GetColumns() const noexcept {
        return columns_;
    }

private:
    std::array<const Column<E>*, sizeof...(Columns)> columns_;
};


template<typename E, typename... Columns>
class CompositeColumn;


template<typename E, typename First, typename... Rest>
class CompositeColumn<E, First, Rest...> :
    public BaseCompositeColumn<E, First, Rest...> {

public:
    using ValueType = std::tuple<typename First::ValueType, typename Rest::ValueType...>;

    static void BindValueToStatement(
        Statement& statement,
        int parameter_index,
        const ValueType& value) {

        ValueTypeTraits<ValueType>::BindValueToStatement(statement, parameter_index, value);
    }

    static ValueType GetValueFromStatement(const Statement& statement, int column_index) {
        return ValueTypeTraits<ValueType>::GetValueFromStatement(statement, column_index);
    }

public:
    CompositeColumn(const First& first, const Rest&... rest) : 
        BaseCompositeColumn<E, First, Rest...>(first, rest...) {

    }
};


template<typename E, typename Single>
class CompositeColumn<E, Single> : public BaseCompositeColumn<E, Single> {
public:
    using ValueType = typename Single::ValueType;

    static void BindValueToStatement(
        Statement& statement,
        int parameter_index,
        const ValueType& value) {

        ValueTypeTraits<ValueType>::BindValueToStatement(statement, parameter_index, value);
    }

    static ValueType GetValueFromStatement(const Statement& statement, int column_index) {
        return ValueTypeTraits<ValueType>::GetValueFromStatement(statement, column_index);
    }

public:
    explicit CompositeColumn(const Single& single) : BaseCompositeColumn<E, Single>(single) {

    }
};


template<typename T>
struct IsCompositeColumnBased {
private:
    template<typename K, typename E, typename... Columns>
    static constexpr bool Test(CompositeColumn<E, Columns...>*) {
        return true;
    }

    template<typename K>
    static constexpr bool Test(...) {
        return false;
    }

public:
    static constexpr bool value = Test<T>((T*)nullptr);
};

template<typename T>
constexpr bool IsCompositeColumnBasedV = IsCompositeColumnBased<T>::value;

}