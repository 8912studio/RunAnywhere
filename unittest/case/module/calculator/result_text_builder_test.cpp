#include <gtest/gtest.h>
#include "module/calculator/result_text_builder.h"

using namespace ra::mod::calculator;

TEST(ResultTextBuilderTest, DecimalWithBitLength) {

    auto test = [](
        std::int64_t value,
        std::optional<int> bit_length,
        bool use_unsigned_type,
        const std::wstring& expected_text) {

        EvaluateResult evaluate_result;
        evaluate_result.decimal_value = value;

        Modifier modifier;
        modifier.base = 10;
        modifier.bit_length = bit_length;
        modifier.use_unsigned_type = use_unsigned_type;

        ResultTextBuilder builder(evaluate_result, modifier);
        return builder.Build().GetCompleteText() == expected_text;
    };

    ASSERT_TRUE(test(100000000000001ll, std::nullopt, false, L"100000000000001"));
    ASSERT_TRUE(test(100000000000001ll, std::nullopt, true, L"100000000000001"));
    ASSERT_TRUE(test(222, 8, false, L"-34"));
    ASSERT_TRUE(test(222, 8, true, L"222"));
    ASSERT_TRUE(test(65000, 16, false, L"-536"));
    ASSERT_TRUE(test(65000, 16, true, L"65000"));
    ASSERT_TRUE(test(4294967280, 32, false, L"-16"));
    ASSERT_TRUE(test(4294967280, 32, true, L"4294967280"));
    ASSERT_TRUE(test(0xfffffffffffffff0ll, 64, false, L"-16"));
    ASSERT_TRUE(test(0xfffffffffffffff0ll, 64, true, L"18446744073709551600"));
    ASSERT_TRUE(test(0xfffffffffffffff0ll, std::nullopt, false, L"-16"));
}


TEST(ResultTextBuilderTest, NonDecimalWithBitLength) {

    auto test = [](
        std::int64_t value, 
        std::optional<int> bit_length, 
        const std::wstring& expected_text) {
    
        EvaluateResult evaluate_result;
        evaluate_result.decimal_value = value;

        Modifier modifier;
        modifier.base = 16;
        modifier.bit_length = bit_length;

        ResultTextBuilder builder(evaluate_result, modifier);
        return builder.Build().GetCompleteText() == expected_text;
    };

    ASSERT_TRUE(test(0xabcdef643216, 8, L"0x16"));
    ASSERT_TRUE(test(0xabcdef643216, 16, L"0x3216"));
    ASSERT_TRUE(test(0xabcdef643216, 32, L"0xef643216"));
    ASSERT_TRUE(test(0xabcdef643216, 64, L"0xabcdef643216"));

    ASSERT_TRUE(test(0xabcd, std::nullopt, L"0xabcd"));
    ASSERT_TRUE(test(0xabcdef643216, std::nullopt, L"0xabcdef643216"));

    std::int64_t value = -static_cast<std::int64_t>(0xffffffff);
    ASSERT_TRUE(test(value, std::nullopt, L"0xffffffff00000001"));
    ASSERT_TRUE(test(value, 8, L"0x1"));
    ASSERT_TRUE(test(value, 16, L"0x1"));
    ASSERT_TRUE(test(value, 32, L"0x1"));
    ASSERT_TRUE(test(value, 64, L"0xffffffff00000001"));
}