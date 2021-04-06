#include <gtest/gtest.h>
#include "module/calculator/parse/common_option_set_parser.h"

using namespace calculator;

TEST(CommonOptionSetParserTest, Empty) {

    auto test = [](const std::wstring& input) {

        ParseContext context{ input };
        ParseResult result{};
        auto status = CommonOptionSetParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        const auto& modifier = result.GetModifier();
        if (modifier.base != 10) {
            return false;
        }

        if (modifier.bit_length != 32) {
            return false;
        }

        if (modifier.use_upper_case) {
            return false;
        }

        return context.GetCurrentIndex() == 0;
    };

    ASSERT_TRUE(test(L""));
    ASSERT_TRUE(test(L","));
    ASSERT_TRUE(test(L"189"));
    ASSERT_TRUE(test(L",a"));
    ASSERT_TRUE(test(L"za"));
}


TEST(CommonOptionSetParserTest, BaseOnly) {

    auto test = [](const std::wstring& input, int expected_base, bool use_upper_case) {

        ParseContext context{ input };
        ParseResult result{};
        auto status = CommonOptionSetParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        const auto& modifier = result.GetModifier();

        if (modifier.base != expected_base) {
            return false;
        }

        if (modifier.bit_length != 32) {
            return false;
        }

        if (modifier.use_upper_case != use_upper_case) {
            return false;
        }

        return context.GetCurrentIndex() == context.GetLength();
    };

    ASSERT_TRUE(test(L"b", 2, false));
    ASSERT_TRUE(test(L"B", 2, true));
    ASSERT_TRUE(test(L"o", 8, false));
    ASSERT_TRUE(test(L"O", 8, true));
    ASSERT_TRUE(test(L"d", 10, false));
    ASSERT_TRUE(test(L"D", 10, true));
    ASSERT_TRUE(test(L"x", 16, false));
    ASSERT_TRUE(test(L"X", 16, true));
    ASSERT_TRUE(test(L"X ", 16, true));
}


TEST(CommonOptionSetParserTest, Combine) {

    auto test = [](
        const std::wstring& input,
        int expected_base, 
        int expected_length, 
        bool use_upper_case) {

        ParseContext context{ input };
        ParseResult result{};
        auto status = CommonOptionSetParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        const auto& modifier = result.GetModifier();

        if (modifier.base != expected_base) {
            return false;
        }

        if (modifier.bit_length != expected_length) {
            return false;
        }

        if (modifier.use_upper_case != use_upper_case) {
            return false;
        }

        return context.GetCurrentIndex() == context.GetLength();
    };

    ASSERT_TRUE(test(L"bb", 2, 8, false));
    ASSERT_TRUE(test(L"bq", 2, 64, false));
    ASSERT_TRUE(test(L"Xw", 16, 16, true));
    ASSERT_TRUE(test(L"dd", 10, 32, false));
}



