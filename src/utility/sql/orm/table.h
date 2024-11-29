#pragma once

#include <span>
#include <string_view>
#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/column.h"
#include "utility/sql/orm/primary_key.h"

namespace ra::utility::sql {

class AbstractTable : zaf::NonCopyableNonMovable {
public:
    AbstractTable() = default;
    virtual ~AbstractTable() = default;

    virtual std::string_view GetName() const noexcept = 0;
    virtual AbstractColumnsView GetAbstractColumns() const noexcept = 0;

    virtual const AbstractPrimaryKey* GetAbstractPrimaryKey() const noexcept {
        return nullptr;
    }
};

}