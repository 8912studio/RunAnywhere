#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/abstract_key.h"

namespace ra::utility::sql {

class AbstractPrimaryKey : public AbstractKey {
public:
    virtual bool IsAutoincrement() const noexcept {
        return false;
    }
};

}