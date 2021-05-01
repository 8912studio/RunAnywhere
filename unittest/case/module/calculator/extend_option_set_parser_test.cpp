#include <gtest/gtest.h>
#include "module/calculator/parse/extend_option_set_parser.h"

using namespace ra::module::calculator;

TEST(ExtendOptionSetParserTest, Unit) {

    auto test = [](const std::wstring& input, NumberUnit expected_unit) {

        ParseContext context{ input };
        ParseResult result;

        auto status = ExtendOptionSetParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        const auto& modifier = result.GetModifier();
        if (modifier.unit != expected_unit) {
            return false;
        }

        if (modifier.highlight_bit.has_value()) {
            return false;
        }

        return context.GetCurrentIndex() == context.GetLength();
    };

    ASSERT_TRUE(test(L"'k", NumberUnit::Kilo ));
    ASSERT_TRUE(test(L"'K", NumberUnit::Kilo));
    ASSERT_TRUE(test(L"'m", NumberUnit::Mega));
    ASSERT_TRUE(test(L"'M", NumberUnit::Mega));
    ASSERT_TRUE(test(L"'g", NumberUnit::Giga));
    ASSERT_TRUE(test(L"'G", NumberUnit::Giga));
    ASSERT_TRUE(test(L"'t", NumberUnit::Tera));
    ASSERT_TRUE(test(L"'T", NumberUnit::Tera));
}


TEST(ExtendOptionSetParserTest, HighlightBit) {

    auto test = [](const std::wstring& input, int expected_bit) {

        ParseContext context{ input };
        ParseResult result;

        auto status = ExtendOptionSetParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        const auto& modifier = result.GetModifier();
        if (modifier.unit != NumberUnit::Normal) {
            return false;
        }

        if (*modifier.highlight_bit != expected_bit) {
            return false;
        }

        return context.GetCurrentIndex() == context.GetLength();
    };

    ASSERT_TRUE(test(L"^0", 0));
    ASSERT_TRUE(test(L"^63", 63));
    ASSERT_TRUE(test(L"^32", 32));
    ASSERT_TRUE(test(L"^23434", 23434));
}


TEST(ExtendOptionSetParserTest, Combine) {

    auto test = [](const std::wstring& input, NumberUnit expected_unit, int expected_bit) {

        ParseContext context{ input };
        ParseResult result;

        auto status = ExtendOptionSetParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        const auto& modifier = result.GetModifier();
        if (modifier.unit != expected_unit) {
            return false;
        }

        if (*modifier.highlight_bit != expected_bit) {
            return false;
        }

        return context.GetCurrentIndex() == context.GetLength();
    };

    ASSERT_TRUE(test(L"'g^9", NumberUnit::Giga, 9));
    ASSERT_TRUE(test(L"^10'k", NumberUnit::Kilo, 10));
    ASSERT_TRUE(test(L"^33  'M", NumberUnit::Mega, 33));
}


TEST(ExtendOptionSetParserTest, Empty) {

    auto test = [](const std::wstring& input) {

        ParseContext context{ input };
        ParseResult result;

        auto status = ExtendOptionSetParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        const auto& modifier = result.GetModifier();
        if (modifier.unit != NumberUnit::Normal) {
            return false;
        }

        if (modifier.highlight_bit.has_value()) {
            return false;
        }

        return context.GetCurrentIndex() == 0;
    };

    ASSERT_TRUE(test(L""));
    ASSERT_TRUE(test(L"abchjk"));
    ASSERT_TRUE(test(L"3289"));
    ASSERT_TRUE(test(L"-#$"));
}


TEST(ExtendOptionSetParserTest, Error) {

    auto test = [](const std::wstring& input) {
    
        ParseContext context{ input };
        ParseResult result;

        auto status = ExtendOptionSetParser::Instance()->Parse(context, result);
        return status == ParseStatus::Error;
    };

    ASSERT_TRUE(test(L"'d"));
    ASSERT_TRUE(test(L"^a"));
    ASSERT_TRUE(test(L"'g^b"));
    ASSERT_TRUE(test(L"^8'a"));
}