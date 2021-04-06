#include <gtest/gtest.h>
#include "number_parser_test_utility.h"
#include "module/calculator/parse/octal_number_parser.h"

TEST(OctalNumberParserTest, Success) {

    auto test = [](const std::wstring& input, const std::wstring& expected_output) {

        return TestNumberParserSuccess(
            *calculator::OctalNumberParser::Instance(),
            input,
            expected_output,
            8);
    };

    ASSERT_TRUE(test(L"00", L"0"));
    ASSERT_TRUE(test(L"01", L"1"));
    ASSERT_TRUE(test(L"007", L"07"));
    ASSERT_TRUE(test(L"05623", L"5623"));
}


TEST(OctalNumberParserTest, Failure) {

    auto test = [](const std::wstring& input, calculator::ParseStatus expected_status) {

        return TestNumberParserFailure(
            *calculator::OctalNumberParser::Instance(),
            input,
            expected_status);
    };

    ASSERT_TRUE(test(L"10384", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"896", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"082", calculator::ParseStatus::Error));
    ASSERT_TRUE(test(L"0b001", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0xffe", calculator::ParseStatus::Mismatched));
    ASSERT_TRUE(test(L"0jkl", calculator::ParseStatus::Mismatched));
}