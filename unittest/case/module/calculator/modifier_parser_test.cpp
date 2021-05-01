#include <gtest/gtest.h>
#include "module/calculator/parse/modifier_parser.h"

using namespace ra::module::calculator;

TEST(ModifierParserTest, Success) {

    auto test = [](const std::wstring& input, int expected_base) {

        ParseContext context{ input };
        ParseResult result{};
        auto status = ModifierParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        if (result.GetModifier().base != expected_base) {
            return false;
        }

        return context.GetCurrentIndex() == context.GetLength();
    };

    ASSERT_TRUE(test(L";b", 2));
    ASSERT_TRUE(test(L";B", 2));
    ASSERT_TRUE(test(L";o", 8));
    ASSERT_TRUE(test(L";O", 8));
    ASSERT_TRUE(test(L";d", 10));
    ASSERT_TRUE(test(L";D", 10));
    ASSERT_TRUE(test(L";x", 16));
    ASSERT_TRUE(test(L";X", 16));
    ASSERT_TRUE(test(L";  X ", 16));
}


TEST(ModifierParserTest, Empty) {

    auto test = [](const std::wstring& input) {

        ParseContext context{ input };
        ParseResult result{};
        auto status = ModifierParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        return result.GetModifier().base == 10;
    };

    ASSERT_TRUE(test(L""));
    ASSERT_TRUE(test(L";"));
    ASSERT_TRUE(test(L"189"));
    ASSERT_TRUE(test(L";a"));
}


TEST(ModifierParserTest, Combine) {

    auto test = [](
        const std::wstring& input, 
        int expected_base, 
        int expected_length,
        NumberUnit unit,
        std::optional<int> highlight_bit) {

        ParseContext context{ input };
        ParseResult result{};
        auto status = ModifierParser::Instance()->Parse(context, result);
        if (status != ParseStatus::Ok) {
            return false;
        }

        if (result.GetModifier().base != expected_base) {
            return false;
        }

        if (result.GetModifier().bit_length != expected_length) {
            return false;
        }

        return context.GetCurrentIndex() == context.GetLength();
    };

    ASSERT_TRUE(test(L";", 10, 32, NumberUnit::Normal, std::nullopt));
    ASSERT_TRUE(test(L";b", 2, 32, NumberUnit::Normal, std::nullopt));
    ASSERT_TRUE(test(L";xb", 16, 8, NumberUnit::Normal, std::nullopt));
    ASSERT_TRUE(test(L";oq", 8, 64, NumberUnit::Normal, std::nullopt));
    ASSERT_TRUE(test(L"; ^3", 10, 32, NumberUnit::Normal, 3));
    ASSERT_TRUE(test(L"; ^23 't", 10, 32, NumberUnit::Tera, 23));
    ASSERT_TRUE(test(L";'G", 10, 32, NumberUnit::Giga, std::nullopt));
    ASSERT_TRUE(test(L";'G^78", 10, 32, NumberUnit::Giga, 78));
}