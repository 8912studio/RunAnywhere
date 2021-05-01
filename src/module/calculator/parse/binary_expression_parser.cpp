#include "module/calculator/parse/binary_expression_parser.h"
#include "module/calculator/parse/expression_parser.h"
#include "module/calculator/parse/parse_context.h"
#include "module/calculator/parse/terminal_parser.h"
#include "module/calculator/parse/operator_node.h"
#include "module/calculator/parse/parse_result.h"

namespace ra::module::calculator {
namespace {

class BinaryOperatorParser : public TerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override {

        OperatorNode::Type type = OperatorNode::Type::None;

        switch (context.GetCurrentChar()) {
        case L'+':
            type = OperatorNode::Type::Plus;
            break;

        case L'-':
            type = OperatorNode::Type::Minus;
            break;

        case L'*': {
            context.Forward();
            if (context.GetCurrentChar() == L'*') {
                type = OperatorNode::Type::Power;
            }
            else {
                context.Backward();
                type = OperatorNode::Type::Multiply;
            }
            break;
        }

        case L'/':
            type = OperatorNode::Type::Divide;
            break;

        default:
            return ParseStatus::Mismatched;
        }

        auto operator_node = std::make_shared<OperatorNode>();
        operator_node->type = type;
        parse_result.AddOperator(operator_node);

        context.Forward();
        return ParseStatus::Ok;
    }
};


class ImplicitMultiplyOperatorParser : public TerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override {

        auto current_char = context.GetCurrentChar();
        if (current_char == 0) {
            return ParseStatus::Mismatched;
        }

        if (current_char == L'(') {

            auto operator_node = std::make_shared<OperatorNode>();
            operator_node->type = OperatorNode::Type::Multiply;
            parse_result.AddOperator(operator_node);
            return ParseStatus::Ok;
        }

        if (context.Backward()) {

            auto previous_char = context.GetCurrentChar();
            context.Forward();

            if (previous_char == L')') {

                auto operator_node = std::make_shared<OperatorNode>();
                operator_node->type = OperatorNode::Type::Multiply;
                parse_result.AddOperator(operator_node);

                return ParseStatus::Ok;
            }
        }

        return ParseStatus::Mismatched;
    }
};

}


BinaryExpressionParser* BinaryExpressionParser::Instance() {

    static BinaryExpressionParser instance;
    return &instance;
}


void BinaryExpressionParser::InitializeParsers() {

    static BinaryOperatorParser binary_operator_parser;
    AddParsers({ &binary_operator_parser, ExpressionParser::Instance() });

    static ImplicitMultiplyOperatorParser implicit_multiply_operator_parser;
    AddParsers({ &implicit_multiply_operator_parser, ExpressionParser::Instance() });

    AddParsers({});
}

}