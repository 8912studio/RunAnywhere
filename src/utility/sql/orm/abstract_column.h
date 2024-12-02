#pragma once

#include <span>
#include <zaf/base/non_copyable.h>
#include "utility/sql/data_type.h"

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

}
