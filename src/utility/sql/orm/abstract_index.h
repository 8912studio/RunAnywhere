#pragma once

#include <span>
#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/abstract_column.h"

namespace ra::utility::sql {

class AbstractIndex : zaf::NonCopyableNonMovable {
public:
    AbstractIndex() = default;
    virtual ~AbstractIndex() = default;

    virtual AbstractColumnsView GetAbstractColumns() const noexcept = 0;
};

using AbstractIndexesView = std::span<const AbstractIndex* const>;

}