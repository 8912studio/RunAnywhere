#pragma once

#include <tuple>
#include "utility/sql/property.h"

namespace ra::utility::sql {

template<typename T, typename... Properties>
class CompositeKey;


template<typename T, typename First, typename... Rest>
class CompositeKey<T, First, Rest...> {
public:
    using ValueType = std::tuple<typename First::ValueType, typename Rest::ValueType...>;
    using PropertyCollectionType = std::tuple<First*, Rest*...>;

    CompositeKey(First& first, Rest&... rest) {
        properties_.push_back(&first);
        properties_.push_back(&rest...);
    }

    const std::vector<Property<T>*> Properties() const {
        return properties_;
    }

private:
    std::vector<Property<T>*> properties_;
};


template<typename T, typename Single>
class CompositeKey<T, Single> {
public:
    using ValueType = typename Single::ValueType;

    explicit CompositeKey(Single& single) {
        properties_.push_back(&single);
    }

    const std::vector<Property<T>*> Properties() const {
        return properties_;
    }

private:
    std::vector<Property<T>*> properties_;
};

}