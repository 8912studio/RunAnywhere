#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename E>
class EntityField : zaf::NonCopyableNonMovable {
public:
    explicit EntityField(std::vector<EntityField<E>*>& registered_fields) {
        registered_fields.push_back(this);
    }

    virtual std::string_view Name() const = 0;

    virtual void BindValueToStatement(
        Statement& statement, 
        int parameter_index, 
        const E& entity) const = 0;

    virtual void GetValueFromStatement(
        const Statement& statement, 
        int index,
        E& entity) const = 0;
};

}