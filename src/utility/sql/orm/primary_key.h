#pragma once

#include <tuple>
#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/field.h"

namespace ra::utility::sql {

class AbstractPrimaryKey : zaf::NonCopyableNonMovable {
public:
    AbstractPrimaryKey() = default;
    virtual ~AbstractPrimaryKey() = default;

    virtual AbstractFieldsView GetAbstractFields() const noexcept = 0;
};


template<typename T, typename... Fields>
class PrimaryKey;


template<typename T, typename First, typename... Rest>
class PrimaryKey<T, First, Rest...> : public AbstractPrimaryKey {
public:
    using ValueType = std::tuple<typename First::ValueType, typename Rest::ValueType...>;

    static void BindValue(Statement& statement, int parameter_index, const ValueType& value) {
        int index = parameter_index;
        std::apply([&statement, &index](const auto& each_value) {
            statement.BindParameter(index, each_value);
            ++index;
        },
        value);
    }

    PrimaryKey(First& first, Rest&... rest) {
        fields_.push_back(&first);
        fields_.push_back(&rest...);
    }

    AbstractFieldsView GetAbstractFields() const noexcept override {
        return { 
            reinterpret_cast<const AbstractField* const*>(fields_.data()),
            fields_.size()
        }; 
    }

    FieldsView<T> Fields() const {
        return fields_;
    }

private:
    std::vector<const Field<T>*> fields_;
};


template<typename T, typename Single>
class PrimaryKey<T, Single> : public AbstractPrimaryKey {
public:
    using ValueType = typename Single::ValueType;

    static void BindValue(Statement& statement, int parameter_index, const ValueType& value) {
        statement.BindParameter(parameter_index, value);
    }

    explicit PrimaryKey(Single& single) {
        fields_.push_back(&single);
    }

    AbstractFieldsView GetAbstractFields() const noexcept override {
        return {
            reinterpret_cast<const AbstractField* const*>(fields_.data()),
            fields_.size()
        };
    }

    FieldsView<T> Fields() const {
        return fields_;
    }

private:
    std::vector<const Field<T>*> fields_;
};


template<typename T, typename... Fields>
auto MakePrimaryKey(Fields&... fields) {
    return PrimaryKey<T, Fields...>(fields...);
}

}