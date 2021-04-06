#include <gtest/gtest.h>
#include "number_parser_test_utility.h"
#include "module/calculator/parse/non_decimal_number_parser.h"

TEST(NonDecimalNumberParserTest, BinaryFailure) {

    auto test = [](const std::wstring& input, calculator::ParseStatus expected_status) {

        return TestNumberParserFailure(
            *calculator::NonDecimalNumberParser::Binary(),
            input,
            expected_status);
    };

    ASSERT_TRUE(test(L"10384", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"896", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0b", calculator::ParseStatus::Error));
    ASSERT_TRUE(test(L"b", calculator::ParseStatus::Error));
    ASSERT_TRUE(test(L"0B", calculator::ParseStatus::Error));
    ASSERT_TRUE(test(L"B", calculator::ParseStatus::Error));
}


TEST(NonDecimalNumberParserTest, HexSuccess) {

    auto test = [](const std::wstring& input, const std::wstring& expected_output) {

        return TestNumberParserSuccess(
            *calculator::NonDecimalNumberParser::Hex(),
            input,
            expected_output,
            16);
    };

    ASSERT_TRUE(test(L"0x3f2E", L"3f2E"));
    ASSERT_TRUE(test(L"xFFeee", L"FFeee"));
    ASSERT_TRUE(test(L"0X0000af", L"0000af"));
    ASSERT_TRUE(test(L"X9a31", L"9a31"));
}


TEST(NonDecimalNumberParserTest, HexFailure) {

    auto test = [](const std::wstring& input, calculator::ParseStatus expected_status) {

        return TestNumberParserFailure(
            *calculator::NonDecimalNumberParser::Hex(),
            input,
            expected_status);
    };

    ASSERT_TRUE(test(L"ab3e22", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"896", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0x", calculator::ParseStatus::Error));
    ASSERT_TRUE(test(L"x", calculator::ParseStatus::Error));
    ASSERT_TRUE(test(L"0X", calculator::ParseStatus::Error));
    ASSERT_TRUE(test(L"X", calculator::ParseStatus::Error));
}