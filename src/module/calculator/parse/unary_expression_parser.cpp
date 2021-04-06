#include "module/calculator/parse/unary_expression_parser.h"
#include "module/calculator/parse/expression_parser.h"
#include "module/calculator/parse/parse_context.h"
#include "module/calculator/parse/terminal_parser.h"
#include "module/calculator/parse/operator_node.h"
#include "module/calculator/parse/parse_result.h"

namespace calculator {

class UnaryOperatorParser : public TerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override {

        OperatorNode::Type operator_type = OperatorNode::Type::None;

        wchar_t current_char = context.GetCurrentChar();
        switch (current_char) {

        case L'+':
            operator_type = OperatorNode::Type::Positive;
            break;

        case L'-':
            operator_type = OperatorNode::Type::Negative;
            break;

        case L'~':
            operator_type = OperatorNode::Type::Not;
            break;

        default:
            return ParseStatus::Mismatched;
        }

        auto operator_node = std::make_shared<OperatorNode>();
        operator_node->type = operator_type;
        parse_result.AddOperator(operator_node);

        context.Forward();
        return ParseStatus::Ok;
    }
};


UnaryExpressionParser* UnaryExpressionParser::Instance() {

    static UnaryExpressionParser instance;
    return &instance;
}


void UnaryExpressionParser::InitializeParsers() {

    static UnaryOperatorParser unary_operator_parser;

    AddParsers({
        &unary_operator_parser,
        ExpressionParser::Instance()
    });
}

}