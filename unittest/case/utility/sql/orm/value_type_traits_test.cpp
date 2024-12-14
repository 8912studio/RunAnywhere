#include <gtest/gtest.h>
#include "utility/sql/orm/value_type/composite_value_type.h"

using namespace ra::utility::sql;

TEST(ORMTest, MapToDataType) {

    ASSERT_EQ(MapToDataTypeV<bool>, DataType::Integer);

    ASSERT_EQ(MapToDataTypeV<char>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<signed char>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<unsigned char>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<char8_t>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<char16_t>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<char32_t>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<wchar_t>, DataType::Integer);

    ASSERT_EQ(MapToDataTypeV<short>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<unsigned short>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<int>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<unsigned int>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<long>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<unsigned long>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<long long>, DataType::Integer);
    ASSERT_EQ(MapToDataTypeV<unsigned long long>, DataType::Integer);

    ASSERT_EQ(MapToDataTypeV<float>, DataType::Float);
    ASSERT_EQ(MapToDataTypeV<double>, DataType::Float);
    ASSERT_EQ(MapToDataTypeV<long double>, DataType::Float);

    ASSERT_EQ(MapToDataTypeV<std::string>, DataType::Text);
}


TEST(ORMTest, IsPrimitiveValueTypeV) {

    ASSERT_TRUE(IsPrimitiveValueTypeV<bool>);

    ASSERT_TRUE(IsPrimitiveValueTypeV<char>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<signed char>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<unsigned char>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<char8_t>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<char16_t>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<char32_t>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<wchar_t>);

    ASSERT_TRUE(IsPrimitiveValueTypeV<short>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<unsigned short>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<int>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<unsigned int>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<long>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<unsigned long>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<long long>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<unsigned long long>);

    ASSERT_TRUE(IsPrimitiveValueTypeV<float>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<double>);
    ASSERT_TRUE(IsPrimitiveValueTypeV<long double>);

    ASSERT_TRUE(IsPrimitiveValueTypeV<std::string>);
}


TEST(ORMTest, IsNullableValueTypeV) {

    ASSERT_TRUE(IsNullableValueTypeV<std::optional<bool>>);

    ASSERT_TRUE(IsNullableValueTypeV<std::optional<char>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<signed char>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<unsigned char>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<char8_t>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<char16_t>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<char32_t>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<wchar_t>>);

    ASSERT_TRUE(IsNullableValueTypeV<std::optional<short>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<unsigned short>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<int>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<unsigned int>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<long>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<unsigned long>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<long long>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<unsigned long long>>);

    ASSERT_TRUE(IsNullableValueTypeV<std::optional<float>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<double>>);
    ASSERT_TRUE(IsNullableValueTypeV<std::optional<long double>>);

    ASSERT_TRUE(IsNullableValueTypeV<std::optional<std::string>>);
}


TEST(ORMTest, IsCompositeValueTypeV) {

    {
        using Type1 = std::tuple<int>;
        ASSERT_TRUE(IsCompositeValueTypeV<Type1>);
    }

    {
        using Type2 = std::tuple<int, std::string>;
        ASSERT_TRUE(IsCompositeValueTypeV<Type2>);
    }
}