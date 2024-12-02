#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/abstract_column.h"

namespace ra::utility::sql {

class AbstractKey : zaf::NonCopyableNonMovable {
public:
    AbstractKey() = default;
    virtual ~AbstractKey() = default;

    virtual AbstractColumnsView GetAbstractColumns() const noexcept = 0;
};

}