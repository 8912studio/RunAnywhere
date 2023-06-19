#include "number_parser_test_utility.h"

using namespace ra::mod::calculator;

bool TestNumberParserSuccess(
    Parser& parser,
    const std::wstring& input,
    const std::wstring& expected_output,
    int expected_base) {

    ParseContext parse_context{ input };
    ParseResult parse_result;

    auto parse_status = parser.Parse(parse_context, parse_result);
    if (parse_status != ParseStatus::Ok) {
        return false;
    }

    if (parse_context.GetCurrentIndex() != input.length()) {
        return false;
    }

    if (parse_context.GetLastParsedLength() != input.length()) {
        return false;
    }

    auto operand_node = dynamic_cast<OperandNode*>(
        parse_result.GetExpressionRootNode().get());

    if (!operand_node) {
        return false;
    }

    if (operand_node->base != expected_base) {
        return false;
    }

    return operand_node->text == expected_output;
}


bool TestNumberParserFailure(
    Parser& parser,
    const std::wstring& input,
    ParseStatus expected_status,
    std::size_t expected_parsed_length) {

    ParseContext parse_context{ input };
    ParseResult parse_result;

    auto parse_status = parser.Parse(parse_context, parse_result);
    if (parse_status != expected_status) {
        return false;
    }

    if (parse_result.GetExpressionRootNode()) {
        return false;
    }

    if (parse_context.GetLastParsedLength() != expected_parsed_length) {
        return false;
    }

    return true;
}