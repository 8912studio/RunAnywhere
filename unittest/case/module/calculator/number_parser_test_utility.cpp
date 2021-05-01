#include "number_parser_test_utility.h"

using namespace ra::module::calculator;

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
    ParseStatus expected_status) {

    ParseContext parse_context{ input };
    ParseResult parse_result;

    auto parse_status = parser.Parse(parse_context, parse_result);
    if (parse_status != expected_status) {
        return false;
    }

    if (parse_result.GetExpressionRootNode()) {
        return false;
    }

    //Parse position should not changed if it is mismatched.
    if (parse_status == ParseStatus::Mismatched) {
        if (parse_context.GetCurrentIndex() != 0) {
            return false;
        }
    }

    return true;
}