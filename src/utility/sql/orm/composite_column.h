#pragma once

#include <array>
#include "utility/sql/orm/column.h"
#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename Entity, typename... Columns>
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

    ColumnsView<Entity> GetColumns() const noexcept {
        return columns_;
    }

private:
    std::array<const Column<Entity>*, sizeof...(Columns)> columns_;
};


template<typename Entity, typename... Columns>
class CompositeColumn;


template<typename Entity, typename First, typename... Rest>
class CompositeColumn<Entity, First, Rest...> : 
    public BaseCompositeColumn<Entity, First, Rest...> {

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

        std::apply([&binder](const auto&... values) {
                (binder(values), ...);
            },
            value);
    }

public:
    CompositeColumn(const First& first, const Rest&... rest) : 
        BaseCompositeColumn<Entity, First, Rest...>(first, rest...) {

    }
};


template<typename Entity, typename Single>
class CompositeColumn<Entity, Single> : public BaseCompositeColumn<Entity, Single> {
public:
    using ValueType = typename Single::ValueType;

    static void BindValueToStatement(
        Statement& statement,
        int parameter_index,
        const ValueType& value) {

        statement.BindParameter(parameter_index, value);
    }

public:
    explicit CompositeColumn(const Single& single) : BaseCompositeColumn<Entity, Single>(single) {

    }
};

}