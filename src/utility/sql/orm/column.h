#pragma once

#include <span>
#include <zaf/base/non_copyable.h>
#include "utility/sql/data_type.h"
#include "utility/sql/statement.h"

namespace ra::utility::sql {

class AbstractColumn : zaf::NonCopyableNonMovable {
public:
    AbstractColumn() = default;
    virtual ~AbstractColumn() = default;

    virtual std::string_view GetName() const noexcept = 0;
    virtual DataType GetDataType() const noexcept = 0;
    virtual bool IsNullable() const noexcept = 0;
};

using AbstractColumnsView = std::span<const AbstractColumn* const>;


template<typename E>
class Column : public AbstractColumn {
public:
    explicit Column(std::vector<const Column<E>*>& registered_columns) {
        registered_columns.push_back(this);
    }

    virtual void BindValueToStatement(
        Statement& statement, 
        int parameter_index, 
        const E& entity) const = 0;

    virtual void GetValueFromStatement(
        const Statement& statement, 
        int index,
        E& entity) const = 0;
};

template<typename E>
using ColumnsView = std::span<const Column<E>* const>;

}