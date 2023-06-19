#include <gtest/gtest.h>
#include "number_parser_test_utility.h"
#include "module/calculator/parse/octal_number_parser.h"

using namespace ra::mod::calculator;

TEST(OctalNumberParserTest, Success) {

    auto test = [](const std::wstring& input, const std::wstring& expected_output) {

        return TestNumberParserSuccess(
            *OctalNumberParser::Instance(),
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

    auto test = [](
        const std::wstring& input,
        ParseStatus expected_status,
        std::size_t expected_parsed_length) {

        return TestNumberParserFailure(
            *OctalNumberParser::Instance(),
            input,
            expected_status,
            expected_parsed_length);
    };

    ASSERT_TRUE(test(L"10384", ParseStatus::Mismatched, 0));
    ASSERT_TRUE(test(L"896", ParseStatus::Mismatched, 0));
    ASSERT_TRUE(test(L"0", ParseStatus::Mismatched, 0));
    ASSERT_TRUE(test(L"082", ParseStatus::Error, 1));
    ASSERT_TRUE(test(L"0b001", ParseStatus::Mismatched, 0));
    ASSERT_TRUE(test(L"0xffe", ParseStatus::Mismatched, 0));
    ASSERT_TRUE(test(L"0jkl", ParseStatus::Mismatched, 0));
}