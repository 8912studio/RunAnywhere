#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename T>
class EntityField : zaf::NonCopyableNonMovable {
public:
    explicit EntityField(std::vector<EntityField<T>*>& registered_fields) {
        registered_fields.push_back(this);
    }

    virtual std::string_view Name() const = 0;

    virtual void BindValueToStatement(
        Statement& statement, 
        int parameter_index, 
        const T& object) const = 0;

    virtual void GetValueFromStatement(
        const Statement& statement, 
        int index, T& object) const = 0;
};

}