#include "module/calculator/parse/expression_parser.h"
#include "module/calculator/parse/binary_expression_parser.h"
#include "module/calculator/parse/number_parser.h"
#include "module/calculator/parse/unary_expression_parser.h"
#include "module/calculator/parse/parse_context.h"
#include "module/calculator/parse/terminal_parser.h"
#include "module/calculator/parse/operator_node.h"
#include "module/calculator/parse/parse_result.h"

namespace ra::module::calculator {

class ParenthesisParser : public TerminalParser {
public:
    ParenthesisParser(OperatorNode::Type operator_type) : operator_type_(operator_type) { }

    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override {

        auto reader = context.BeginRead();
        if (reader.GetChar() != GetParenthesisChar()) {

            if (operator_type_ == OperatorNode::Type::RightParenthesis) {
                context.SetParseErrorReason(ParseErrorReason::MismatchedParenthesis);
            }

            return ParseStatus::Mismatched;
        }

        reader.Forward();

        if (operator_type_ == OperatorNode::Type::LeftParenthesis) {
            context.PushParenthesis();
        }
        else {
            context.PopParenthesis();
        }

        auto operator_node = std::make_shared<OperatorNode>();
        operator_node->type = operator_type_;
        parse_result.AddOperator(operator_node);

        return ParseStatus::Ok;
    }

private:
    wchar_t GetParenthesisChar() const {
        return operator_type_ == OperatorNode::Type::LeftParenthesis ? L'(' : L')';
    }

private:
    OperatorNode::Type operator_type_;
};


ExpressionParser* ExpressionParser::Instance() {

    static ExpressionParser instance;
    return &instance;
}


void ExpressionParser::InitializeParsers() {

    AddParsers({
        NumberParser::Instance(),
        BinaryExpressionParser::Instance()
    });

    AddParsers({
        UnaryExpressionParser::Instance(),
        BinaryExpressionParser::Instance()
    });

    static ParenthesisParser left_parenthesis_parser(OperatorNode::Type::LeftParenthesis);
    static ParenthesisParser right_parenthesis_parser(OperatorNode::Type::RightParenthesis);

    AddParsers({
        &left_parenthesis_parser,
        ExpressionParser::Instance(),
        &right_parenthesis_parser,
        BinaryExpressionParser::Instance()
    });
}

}