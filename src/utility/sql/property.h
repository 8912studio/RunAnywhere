#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename T>
class Property : zaf::NonCopyableNonMovable {
public:
    explicit Property(std::vector<Property<T>*>& registered_properties) {
        registered_properties.push_back(this);
    }

    virtual std::string Name() const = 0;
    virtual void BindValueToStatement(Statement& statement, int column_index, const T& object) = 0;
    virtual void GetValueFromStatement(const Statement& statement, int index, T& object) = 0;
};

}