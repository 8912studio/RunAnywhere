#include <gtest/gtest.h>
#include "number_parser_test_utility.h"
#include "module/calculator/parse/decimal_number_parser.h"

using namespace ra::mod::calculator;

TEST(DecimalNumberParserTest, Success) {

    auto test = [](const std::wstring& input, const std::wstring& expected_output) {

        return TestNumberParserSuccess(
            *DecimalNumberParser::Instance(), 
            input, 
            expected_output, 
            10);
    };

    ASSERT_TRUE(test(L"89", L"89"));
    ASSERT_TRUE(test(L"89.", L"89"));
    ASSERT_TRUE(test(L".89", L"0.89"));
    ASSERT_TRUE(test(L"8.9", L"8.9"));
}


TEST(DecimalNumberParserTest, MultipleDecimalPoint) {

    ParseContext context{ L"23.44.5" };
    ParseResult result;
    auto status = DecimalNumberParser::Instance()->Parse(context, result);
    ASSERT_EQ(status, ParseStatus::Ok);
    ASSERT_EQ(context.GetCurrentIndex(), 5);
    ASSERT_EQ(context.GetLastParsedLength(), 5);

    auto operand_node = std::dynamic_pointer_cast<OperandNode>(result.GetExpressionRootNode());
    ASSERT_NE(operand_node, nullptr);
    ASSERT_EQ(operand_node->text, L"23.44");
}


TEST(DecimalNumberParserTest, Failure) {

    auto test = [](
        const std::wstring& input, 
        ParseStatus expected_status, 
        std::size_t expected_parsed_length) {
    
        return TestNumberParserFailure(
            *DecimalNumberParser::Instance(),
            input, 
            expected_status,
            expected_parsed_length);
    };

    ASSERT_TRUE(test(L"", ParseStatus::Mismatched, 0));
    ASSERT_TRUE(test(L"abce", ParseStatus::Mismatched, 0));
    ASSERT_TRUE(test(L".", ParseStatus::Error, 1));
}


TEST(DecimalNumberParserTest, Unit) {

    auto test = [](
        const std::wstring& input,
        const std::wstring& expected_output,
        NumberUnit expected_unit) {

        ParseContext parse_context{ input };
        ParseResult parse_result;

        auto parse_status = DecimalNumberParser::Instance()->Parse(parse_context, parse_result);
        if (parse_status != ParseStatus::Ok) {
            return false;
        }

        auto operand_node = dynamic_cast<OperandNode*>(
            parse_result.GetExpressionRootNode().get());

        if (operand_node->text != expected_output) {
            return false;
        }

        if (operand_node->unit != expected_unit) {
            return false;
        }

        if (parse_context.GetCurrentIndex() != parse_context.GetLength()) {
            return false;
        }

        return parse_context.GetLastParsedLength() == input.length();
    };

    ASSERT_TRUE(test(L"8k", L"8", NumberUnit::Kilo));
    ASSERT_TRUE(test(L"8K", L"8", NumberUnit::Kilo));
    ASSERT_TRUE(test(L"8m", L"8", NumberUnit::Mega));
    ASSERT_TRUE(test(L"8M", L"8", NumberUnit::Mega));
    ASSERT_TRUE(test(L"8g", L"8", NumberUnit::Giga));
    ASSERT_TRUE(test(L"8G", L"8", NumberUnit::Giga));
    ASSERT_TRUE(test(L"8t", L"8", NumberUnit::Tera));
    ASSERT_TRUE(test(L"8T", L"8", NumberUnit::Tera));
    ASSERT_TRUE(test(L"10.8M", L"10.8", NumberUnit::Mega));
}
