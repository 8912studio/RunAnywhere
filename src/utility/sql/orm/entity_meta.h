#pragma once

#include <span>
#include <string_view>
#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/field.h"
#include "utility/sql/orm/primary_key.h"

template<typename T>
class sql__EntityMeta;

namespace ra::utility::sql {

class AbstractEntityMeta : zaf::NonCopyableNonMovable {
public:
    AbstractEntityMeta() = default;
    virtual ~AbstractEntityMeta() = default;

    virtual std::string_view GetName() const noexcept = 0;
    virtual AbstractFieldsView GetAllAbstractFields() const noexcept = 0;

    virtual const AbstractPrimaryKey* GetAbstractPrimaryKey() const noexcept {
        return nullptr;
    }
};

template<typename T>
using EntityMeta = sql__EntityMeta<T>;

}