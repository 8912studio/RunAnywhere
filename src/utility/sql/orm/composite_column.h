#pragma once

#include <array>
#include "utility/sql/orm/column.h"
#include "utility/sql/orm/field_traits.h"
#include "utility/sql/statement.h"

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

        int index = parameter_index;
        auto binder = [&statement, &index](const auto& value) {
            statement.BindParameter(index, value);
            ++index;
        };

        std::apply(
            [&binder](const auto&... values) {
                (binder(values), ...);
            },
            value);
    }

    static ValueType GetValueFromStatement(const Statement& statement, int column_index) {
        
        int index = column_index;
        auto getter = [&statement, &index](auto& value) {
            value = sql::GetValueFromStatement<std::decay_t<decltype(value)>>(statement, index);
            ++index;
        };

        ValueType result{};
        std::apply(
            [&getter](auto&... values) {
                (getter(values), ...);
            }, 
            result);

        return result;
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

        statement.BindParameter(parameter_index, value);
    }

    static ValueType GetValueFromStatement(const Statement& statement, int column_index) {
        return sql::GetValueFromStatement<ValueType>(statement, column_index);
    }

public:
    explicit CompositeColumn(const Single& single) : BaseCompositeColumn<E, Single>(single) {

    }
};


template<typename T>
struct IsCompositeColumn : std::false_type { };

template<typename E, typename... Columns>
struct IsCompositeColumn<CompositeColumn<E, Columns...>> : std::true_type { };

template<typename T>
constexpr bool IsCompositeColumnV = IsCompositeColumn<T>::value;

}