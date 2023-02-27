#include "utility/number_parsing.h"
#include "module/calculator/evaluate/evaluator.h"
#include "module/calculator/parse/decimal_number_parser.h"
#include "module/calculator/parse/non_decimal_number_parser.h"

using namespace ra::module;

namespace ra::utility {
namespace {

NumberParseStatus ParseNumberWithParser(
    const std::wstring& input,
    calculator::Parser* parser,
    std::shared_ptr<calculator::SyntaxNode>& syntax_node) {

    calculator::ParseContext context(input);
    calculator::ParseResult result;
    auto status = parser->Parse(context, result);

    if (status == calculator::ParseStatus::Mismatched) {
        return NumberParseStatus::NotNumber;
    }

    //The whole text must be parsed completely, or it is not a number.
    if (context.GetCurrentIndex() != context.GetLength()) {
        return NumberParseStatus::NotNumber;
    }

    //There is error during parsing, means that the number is incomplete.
    if (status == calculator::ParseStatus::Error) {
        return NumberParseStatus::Incomplete;
    }

    //Parse success.
    syntax_node = result.GetExpressionRootNode();
    return NumberParseStatus::OK;
}

}


NumberParseStatus ParseNumber(
    const std::wstring& text,
    module::calculator::EvaluateResult& result) {

    std::shared_ptr<calculator::SyntaxNode> syntax_node;

    //Try to parse number in decimal.
    auto status = ParseNumberWithParser(
        text,
        calculator::DecimalNumberParser::Instance(),
        syntax_node);

    if (status == NumberParseStatus::NotNumber) {

        //If failed, try to parse number in hexadecimal again.
        status = ParseNumberWithParser(
            text,
            calculator::NonDecimalNumberParser::Hex(),
            syntax_node);
    }

    if (status != NumberParseStatus::OK) {
        return status;
    }

    auto evaluator = calculator::Evaluator::Create(syntax_node);
    if (!evaluator) {
        return NumberParseStatus::NotNumber;
    }

    auto evaluate_status = evaluator->Evaluate(result);
    if (evaluate_status != calculator::EvaluateStatus::Ok) {
        return NumberParseStatus::NotNumber;
    }

    return NumberParseStatus::OK;
}

}