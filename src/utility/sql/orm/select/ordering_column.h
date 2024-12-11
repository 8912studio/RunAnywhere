#pragma once

#include "utility/sql/orm/column.h"

namespace ra::utility::sql {

constexpr int UnspecificOrder = 0;
constexpr int AscOrder = 1;
constexpr int DescOrder = 2;

template<typename Column, int Order = UnspecificOrder>
class OrderingColumn {
public:
    explicit OrderingColumn(const Column& column) : column_(&column) { 
    
    }

    std::string BuildSQL() const {

        std::string result{ column_->GetName() };
        if constexpr (Order == AscOrder) {
            result += " asc";
        }
        else if constexpr (Order == DescOrder) {
            result += " desc";
        }
        return result;
    }

private:
    const Column* column_{};
};


template <typename T>
struct IsOrderingColumn : std::false_type {};

template <typename Column, int Order>
struct IsOrderingColumn<OrderingColumn<Column, Order>> : std::true_type { };

template <typename T>
inline constexpr bool IsOrderingColumnV = IsOrderingColumn<T>::value;


struct ColumnTag {};
struct OrderingColumnTag {};

template<typename T>
using ToOrderingColumnTag = std::conditional_t<
    IsOrderingColumnV<std::decay_t<T>>,
    OrderingColumnTag,
    ColumnTag
>;

template<typename Column>
auto ToOrderingColumnImpl(const Column& column, ColumnTag) {
    return OrderingColumn{ column };
}

template<typename Column>
auto ToOrderingColumnImpl(const Column& column, OrderingColumnTag) {
    return column;
}

template<typename Column>
auto ToOrderingColumn(const Column& column) {
    return ToOrderingColumnImpl(column, ToOrderingColumnTag<Column>{});
}


template<typename... Columns>
auto MakeOrderingColumns(const Columns&... columns) {
    return std::make_tuple(ToOrderingColumn(columns)...);
}

}