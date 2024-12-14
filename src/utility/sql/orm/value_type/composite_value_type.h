#pragma once

#include "utility/sql/orm/value_type/nullable_value_type.h"
#include "utility/sql/orm/value_type/primitive_value_type.h"

namespace ra::utility::sql {

template<typename... T>
struct AreAllPrimitiveOrNullableValueTypes : std::false_type {};

template<>
struct AreAllPrimitiveOrNullableValueTypes<> : std::true_type {};

template<typename T, typename... Rest>
struct AreAllPrimitiveOrNullableValueTypes<T, Rest...> {
    static constexpr bool value =
        (IsPrimitiveValueTypeV<T> || IsNullableValueTypeV<T>) &&
        AreAllPrimitiveOrNullableValueTypes<Rest...>::value;
};


template<typename T>
struct IsCompositeValueType : std::false_type {};

template<typename... Types>
struct IsCompositeValueType<std::tuple<Types...>> {
    static constexpr bool value = AreAllPrimitiveOrNullableValueTypes<Types...>::value;
};

template<typename T>
constexpr bool IsCompositeValueTypeV = IsCompositeValueType<T>::value;


template<typename T>
struct ValueTypeTraits<T, std::enable_if_t<IsCompositeValueTypeV<T>>> {

    static constexpr std::size_t PlaceholderCount = std::tuple_size<T>::value;

    static int BindValueToStatement(Statement& statement, int parameter_index, const T& value) {

        int index = parameter_index;
        auto binder = [&statement, &index](const auto& value) {
            ValueTypeTraits<std::decay_t<decltype(value)>>::BindValueToStatement(
                statement, 
                index, 
                value);
            ++index;
        };

        std::apply(
            [&binder](const auto&... values) {
                (binder(values), ...);
            },
            value);

        return index;
    }

    static T GetValueFromStatement(const Statement& statement, int column_index) {

        int index = column_index;
        auto getter = [&statement, &index](auto& value) {
            value = ValueTypeTraits<std::decay_t<decltype(value)>>::GetValueFromStatement(
                statement, 
                index);
            ++index;
        };

        T result{};
        std::apply(
            [&getter](auto&... values) {
                (getter(values), ...);
            },
            result);

        return result;
    }
};

}