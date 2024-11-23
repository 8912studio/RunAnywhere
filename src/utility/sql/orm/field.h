#pragma once

#include <span>
#include <zaf/base/non_copyable.h>
#include "utility/sql/data_type.h"
#include "utility/sql/statement.h"

namespace ra::utility::sql {

class AbstractField : zaf::NonCopyableNonMovable {
public:
    AbstractField() = default;
    virtual ~AbstractField() = default;

    virtual std::string_view Name() const = 0;
    virtual sql::DataType DataType() const = 0;
};

using AbstractFieldsView = std::span<const AbstractField* const>;


template<typename E>
class Field : public AbstractField {
public:
    explicit Field(std::vector<const Field<E>*>& registered_fields) {
        registered_fields.push_back(this);
    }

    virtual void BindValueToStatement(
        Statement& statement, 
        int parameter_index, 
        const E& entity) const = 0;

    virtual void GetValueFromStatement(
        const Statement& statement, 
        int index,
        E& entity) const = 0;
};

template<typename E>
using FieldsView = std::span<const Field<E>* const>;

}