#include <gtest/gtest.h>
#include "number_parser_test_utility.h"
#include "module/calculator/parse/non_decimal_number_parser.h"

using namespace ra::module::calculator;

TEST(NonDecimalNumberParserTest, BinaryFailure) {

    auto test = [](const std::wstring& input, ParseStatus expected_status) {

        return TestNumberParserFailure(
            *NonDecimalNumberParser::Binary(),
            input,
            expected_status);
    };

    ASSERT_TRUE(test(L"10384", ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"896", ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0", ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0b", ParseStatus::Error));
    ASSERT_TRUE(test(L"b", ParseStatus::Error));
    ASSERT_TRUE(test(L"0B", ParseStatus::Error));
    ASSERT_TRUE(test(L"B", ParseStatus::Error));
}


TEST(NonDecimalNumberParserTest, HexSuccess) {

    auto test = [](const std::wstring& input, const std::wstring& expected_output) {

        return TestNumberParserSuccess(
            *NonDecimalNumberParser::Hex(),
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

    auto test = [](const std::wstring& input, ParseStatus expected_status) {

        return TestNumberParserFailure(
            *NonDecimalNumberParser::Hex(),
            input,
            expected_status);
    };

    ASSERT_TRUE(test(L"ab3e22", ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"896", ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0", ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0x", ParseStatus::Error));
    ASSERT_TRUE(test(L"x", ParseStatus::Error));
    ASSERT_TRUE(test(L"0X", ParseStatus::Error));
    ASSERT_TRUE(test(L"X", ParseStatus::Error));
}