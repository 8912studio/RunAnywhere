#pragma once

#include <tuple>
#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/column.h"

namespace ra::utility::sql {

class AbstractPrimaryKey : zaf::NonCopyableNonMovable {
public:
    AbstractPrimaryKey() = default;
    virtual ~AbstractPrimaryKey() = default;

    virtual AbstractColumnsView GetAbstractColumns() const noexcept = 0;

    virtual bool IsAutoincrement() const noexcept {
        return false;
    }
};


template<typename E, typename... Columns>
class PrimaryKey;


template<typename E, typename First, typename... Rest>
class PrimaryKey<E, First, Rest...> : public AbstractPrimaryKey {
public:
    static_assert(
        !First::ValueTraits::IsNullable && !(Rest::ValueTraits::IsNullable && ...), 
        "Primary key cannot be nullable.");

    using ValueType = std::tuple<typename First::ValueType, typename Rest::ValueType...>;

    static void BindValue(Statement& statement, int parameter_index, const ValueType& value) {
        int index = parameter_index;
        std::apply([&statement, &index](const auto& each_value) {
            statement.BindParameter(index, each_value);
            ++index;
        },
        value);
    }

    PrimaryKey(First& first, Rest&... rest) {
        columns_.push_back(&first);
        columns_.push_back(&rest...);
    }

    AbstractColumnsView GetAbstractColumns() const noexcept override {
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
class PrimaryKey<E, Single> : public AbstractPrimaryKey {
public:
    static_assert(!Single::ValueTraits::IsNullable, "Primary key cannot be nullable.");

    using ValueType = typename Single::ValueType;

    static void BindValue(Statement& statement, int parameter_index, const ValueType& value) {
        statement.BindParameter(parameter_index, value);
    }

    explicit PrimaryKey(const Single& single) : column_(&single) {

    }

    AbstractColumnsView GetAbstractColumns() const noexcept override {
        return { reinterpret_cast<const AbstractColumn* const*>(&column_), 1 };
    }

    ColumnsView<E> GetColumns() const noexcept {
        return { reinterpret_cast<const Column<E>* const*>(&column_), 1 };
    }

private:
    const Single* column_{};
};


template<typename E, typename... Columns>
auto MakePrimaryKey(Columns&... columns) {
    return PrimaryKey<E, Columns...>(columns...);
}

}