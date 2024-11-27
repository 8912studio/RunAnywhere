#pragma once

#include <type_traits>

namespace ra::utility::sql {

template<typename T, typename = void>
struct HasPrimaryKey : std::false_type { };

template<typename T>
struct HasPrimaryKey<T, std::void_t<typename T::PrimaryKeyType>> : std::true_type { };

template<typename T>
constexpr bool HasPrimaryKeyV = HasPrimaryKey<T>::value;


template<typename T, typename = void>
struct HasAutoincrementPrimaryKey : std::false_type {};

template<typename T>
struct HasAutoincrementPrimaryKey<T, std::void_t<typename T::PrimaryKeyType::AutoincrementTag>> :
    std::true_type {};

template<typename T>
constexpr bool HasAutoincrementPrimaryKeyV = HasAutoincrementPrimaryKey<T>::value;

}