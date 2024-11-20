#pragma once

#include <tuple>
#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/entity_field.h"

namespace ra::utility::sql {

template<typename T, typename... Fields>
class EntityKey;


template<typename T, typename First, typename... Rest>
class EntityKey<T, First, Rest...> : zaf::NonCopyableNonMovable {
public:
    using ValueType = std::tuple<typename First::ValueType, typename Rest::ValueType...>;

    EntityKey(First& first, Rest&... rest) {
        fields_.push_back(&first);
        fields_.push_back(&rest...);
    }

    const std::vector<EntityField<T>*> Fields() const {
        return fields_;
    }

private:
    std::vector<EntityField<T>*> fields_;
};


template<typename T, typename Single>
class EntityKey<T, Single> : zaf::NonCopyableNonMovable {
public:
    using ValueType = typename Single::ValueType;

    explicit EntityKey(Single& single) {
        fields_.push_back(&single);
    }

    const std::vector<EntityField<T>*>& Fields() const {
        return fields_;
    }

private:
    std::vector<EntityField<T>*> fields_;
};


template<typename T, typename... Fields>
auto MakeEntityKey(Fields&... fields) {
    return EntityKey<T, Fields...>(fields...);
}

}