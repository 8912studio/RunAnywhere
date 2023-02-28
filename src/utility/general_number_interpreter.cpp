#include "utility/general_number_interpreter.h"
#include "module/calculator/evaluate/evaluator.h"
#include "module/calculator/parse/decimal_number_parser.h"
#include "module/calculator/parse/non_decimal_number_parser.h"
#include "module/calculator/parse/non_terminal_parser.h"
#include "module/calculator/parse/unary_operator_parser.h"

namespace ra::utility {
namespace {

class SupportNegativeDecimalParser : public module::calculator::NonTerminalParser {
public:
    static module::calculator::Parser* Instance() {
        static SupportNegativeDecimalParser instance;
        return &instance;
    }

private:
    void InitializeParsers() override {

        unary_operator_parser_ = module::calculator::UnaryOperatorParser::Create({
            module::calculator::OperatorNode::Type::Negative 
        });

        AddParsers({ module::calculator::NonDecimalNumberParser::Hex() });
        AddParsers({ module::calculator::DecimalNumberParser::Instance() });
        AddParsers({
            unary_operator_parser_.get(),
            module::calculator::DecimalNumberParser::Instance(),
        });
    }

private:
    SupportNegativeDecimalParser() = default;

private:
    std::unique_ptr<module::calculator::UnaryOperatorParser> unary_operator_parser_;
};


class NotSupportNegativeDecimalParser : public module::calculator::NonTerminalParser {
public:
    static module::calculator::Parser* Instance() {
        static NotSupportNegativeDecimalParser instance;
        return &instance;
    }

protected:
    void InitializeParsers() override {
        AddParsers({ module::calculator::NonDecimalNumberParser::Hex() });
        AddParsers({ module::calculator::DecimalNumberParser::Instance() });
    }

private:
    NotSupportNegativeDecimalParser() = default;
};

}


GeneralNumberInterpreter::GeneralNumberInterpreter(bool supports_negative_decimal) {

    parser_ = 
        supports_negative_decimal ? 
        SupportNegativeDecimalParser::Instance() :
        NotSupportNegativeDecimalParser::Instance();
}


GeneralNumberInterpreter::Status GeneralNumberInterpreter::Interpret(
    const std::wstring& input, 
    module::calculator::EvaluateResult& result) {

    std::shared_ptr<module::calculator::SyntaxNode> syntax_node;
    auto status = Parse(input, syntax_node);
    if (status != Status::OK) {
        return status;
    }

    auto evaluator = module::calculator::Evaluator::Create(syntax_node);
    if (!evaluator) {
        return Status::NotNumber;
    }

    auto evaluate_status = evaluator->Evaluate(result);
    if (evaluate_status != module::calculator::EvaluateStatus::Ok) {
        return Status::NotNumber;
    }

    return Status::OK;
}


GeneralNumberInterpreter::Status GeneralNumberInterpreter::Parse(
    const std::wstring& input,
    std::shared_ptr<module::calculator::SyntaxNode>& syntax_node) {

    module::calculator::ParseContext context(input);
    module::calculator::ParseResult result;
    auto status = parser_->Parse(context, result);

    if (status == module::calculator::ParseStatus::Mismatched) {
        return Status::NotNumber;
    }

    //The whole text must be parsed completely, or it is not a number.
    if (context.GetCurrentIndex() != context.GetLength()) {
        return Status::NotNumber;
    }

    //There is error during parsing, means that the number is incomplete.
    if (status == module::calculator::ParseStatus::Error) {
        return Status::Incomplete;
    }

    //Parse success.
    syntax_node = result.GetExpressionRootNode();
    return Status::OK;
}

}