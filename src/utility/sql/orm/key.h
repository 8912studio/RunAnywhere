#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/column.h"
#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class Key;


template<typename E, typename First, typename... Rest>
class Key<E, First, Rest...> : zaf::NonCopyableNonMovable {
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
    Key(First& first, Rest&... rest) {
        columns_.push_back(&first);
        columns_.push_back(&rest...);
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
    std::vector<const Column<E>*> columns_;
};


template<typename E, typename Single>
class Key<E, Single> : zaf::NonCopyableNonMovable {
public:
    using ValueType = typename Single::ValueType;

    static void BindValueToStatement(
        Statement& statement,
        int parameter_index,
        const ValueType& value) {

        statement.BindParameter(parameter_index, value);
    }

    explicit Key(const Single& single) : column_(&single) {

    }

    AbstractColumnsView GetAbstractColumns() const noexcept {
        return { reinterpret_cast<const AbstractColumn* const*>(&column_), 1 };
    }

    ColumnsView<E> GetColumns() const noexcept {
        return { reinterpret_cast<const Column<E>*const*>(&column_), 1 };
    }

private:
    const Single* column_{};
};

}