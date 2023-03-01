#include "utility/general_number_interpreter.h"
#include "module/calculator/evaluate/evaluator.h"
#include "module/calculator/parse/decimal_number_parser.h"
#include "module/calculator/parse/non_terminal_parser.h"
#include "module/calculator/parse/number_parser.h"
#include "module/calculator/parse/unary_operator_parser.h"

namespace ra::utility {
namespace {

class GeneralNumberParser : public module::calculator::NonTerminalParser {
public:
    static GeneralNumberParser* SupportsNegativeDecimal() {
        static GeneralNumberParser instance{ true };
        return &instance;
    }

    static GeneralNumberParser* NotSupportNegativeDecimal() {
        static GeneralNumberParser instance{ false };
        return &instance;
    }

protected:
    void InitializeParsers() override {

        number_parser_ = module::calculator::NumberParser::Create({
            module::calculator::NumberParser::NumberType::Decimal,
            module::calculator::NumberParser::NumberType::Hex,
        });

        AddParsers({ number_parser_.get() });

        if (supports_negative_decimal_) {

            unary_operator_parser_ = module::calculator::UnaryOperatorParser::Create({
                module::calculator::OperatorNode::Type::Negative
            });

            AddParsers({
                unary_operator_parser_.get(),
                module::calculator::DecimalNumberParser::Instance(),
            });
        }
    }

private:
    explicit GeneralNumberParser(bool supports_negative_decimal) :
        supports_negative_decimal_(supports_negative_decimal) {

    }

private:
    bool supports_negative_decimal_{};

    std::unique_ptr<module::calculator::NumberParser> number_parser_;
    std::unique_ptr<module::calculator::UnaryOperatorParser> unary_operator_parser_;
};

}


GeneralNumberInterpreter::GeneralNumberInterpreter(bool supports_negative_decimal) {

    parser_ = 
        supports_negative_decimal ? 
        GeneralNumberParser::SupportsNegativeDecimal() :
        GeneralNumberParser::NotSupportNegativeDecimal();
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