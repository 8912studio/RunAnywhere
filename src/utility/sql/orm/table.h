#pragma once

#include <span>
#include <string_view>
#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/column.h"
#include "utility/sql/orm/primary_key.h"

template<typename T>
class sql__Table;

namespace ra::utility::sql {

class AbstractTable : zaf::NonCopyableNonMovable {
public:
    AbstractTable() = default;
    virtual ~AbstractTable() = default;

    virtual std::string_view GetName() const noexcept = 0;
    virtual AbstractColumnsView GetAllAbstractFields() const noexcept = 0;

    virtual const AbstractPrimaryKey* GetAbstractPrimaryKey() const noexcept {
        return nullptr;
    }
};

template<typename T>
using Table = sql__Table<T>;

}