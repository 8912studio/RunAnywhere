#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/abstract_column.h"

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

}