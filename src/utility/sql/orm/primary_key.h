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


template<typename T, typename... Columns>
class PrimaryKey;


template<typename T, typename First, typename... Rest>
class PrimaryKey<T, First, Rest...> : public AbstractPrimaryKey {
public:
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

    ColumnsView<T> GetColumns() const noexcept {
        return columns_;
    }

private:
    std::vector<const Column<T>*> columns_;
};


template<typename T, typename Single>
class PrimaryKey<T, Single> : public AbstractPrimaryKey {
public:
    using ValueType = typename Single::ValueType;

    static void BindValue(Statement& statement, int parameter_index, const ValueType& value) {
        statement.BindParameter(parameter_index, value);
    }

    explicit PrimaryKey(const Single& single) : column_(&single) {

    }

    PrimaryKey(const Single& single, bool is_autoincrement) : 
        column_(&single), 
        is_autoincrement_(is_autoincrement) {

    }

    AbstractColumnsView GetAbstractColumns() const noexcept override {
        return { reinterpret_cast<const AbstractColumn* const*>(&column_), 1 };
    }

    ColumnsView<T> GetColumns() const noexcept {
        return { &column_, 1 };
    }

    bool IsAutoincrement() const noexcept override {
        return is_autoincrement_;
    }

private:
    const Single* column_{};
    bool is_autoincrement_{};
};


template<typename T, typename... Columns>
auto MakePrimaryKey(Columns&... columns) {
    return PrimaryKey<T, Columns...>(columns...);
}

}