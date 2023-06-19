#include "module/calculator/parse/binary_expression_parser.h"
#include "module/calculator/parse/expression_parser.h"
#include "module/calculator/parse/parse_context.h"
#include "module/calculator/parse/terminal_parser.h"
#include "module/calculator/parse/operator_node.h"
#include "module/calculator/parse/parse_result.h"

namespace ra::mod::calculator {
namespace {

class BinaryOperatorParser : public TerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override {

        auto reader = context.BeginRead();

        auto operator_type = ParseSimpleOperator(reader);
        if (operator_type == OperatorNode::Type::None) {

            operator_type = ParseComplexOperator(reader);

            if (operator_type == OperatorNode::Type::None) {
                return ParseStatus::Mismatched;
            }
        }

        auto operator_node = std::make_shared<OperatorNode>();
        operator_node->type = operator_type;
        parse_result.AddOperator(operator_node);

        return ParseStatus::Ok;
    }

private:
    OperatorNode::Type ParseSimpleOperator(ParseReader& reader) {

        OperatorNode::Type result{ OperatorNode::Type::None };

        auto ch = reader.GetChar();
        switch (ch) {
        case L'+':
            result = OperatorNode::Type::Plus;
            break;
        case L'-':
            result = OperatorNode::Type::Minus;
            break;
        case L'/':
            result = OperatorNode::Type::Divide;
            break;
        case L'&':
            result = OperatorNode::Type::And;
            break;
        case L'|':
            result = OperatorNode::Type::Or;
            break;
        case L'^':
            result = OperatorNode::Type::Xor;
            break;
        default:
            break;
        }

        if (result != OperatorNode::Type::None) {
            reader.Forward();
        }

        return result;
    }

    OperatorNode::Type ParseComplexOperator(ParseReader& reader) {

        OperatorNode::Type result{ OperatorNode::Type::None };

        auto ch = reader.GetChar();
        if (ch == L'*') {

            reader.Forward();

            ch = reader.GetChar();
            if (ch == L'*') {

                reader.Forward();
                result = OperatorNode::Type::Power;
            }
            else {
                result = OperatorNode::Type::Multiply;
            }
        }
        else if (ch == L'<') {

            reader.Forward();

            ch = reader.GetChar();
            if (ch == L'<') {

                reader.Forward();
                result = OperatorNode::Type::LeftShift;
            }
        }
        else if (ch == L'>') {

            reader.Forward();

            ch = reader.GetChar();
            if (ch == L'>') {

                reader.Forward();
                result = OperatorNode::Type::RightShift;
            }
        }

        return result;
    }
};


class HeadImplicitMultiplyOperatorParser : public TerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override {

        auto current_char = context.GetCurrentChar();
        if (current_char != L'(') {
            return ParseStatus::Mismatched;
        }

        //No need to forward context.

        auto operator_node = std::make_shared<OperatorNode>();
        operator_node->type = OperatorNode::Type::Multiply;
        parse_result.AddOperator(operator_node);
        return ParseStatus::Ok;
    }
};


class TailImplicitMultiplyOperatorParser : public NonTerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& result) override {

        auto previous_char = context.GetCharAtOffset(-1);
        if (!previous_char) {
            return ParseStatus::Mismatched;
        }

        if (*previous_char != L')') {
            return ParseStatus::Mismatched;
        }

        ParseResult expression_parse_result;
        auto parse_status = ExpressionParser::Instance()->Parse(context, expression_parse_result);
        if (parse_status != ParseStatus::Ok) {
            return parse_status;
        }

        auto operand_node = 
            std::dynamic_pointer_cast<OperandNode>(expression_parse_result.GetExpressionRootNode());

        if (!operand_node) {
            return ParseStatus::Error;
        }

        auto operator_node = std::make_shared<OperatorNode>();
        operator_node->type = OperatorNode::Type::Multiply;
        result.AddOperator(operator_node);
        result.AddOperand(operand_node);
        return ParseStatus::Ok;
    }

protected:
    void InitializeParsers() override {
        //Do nothing.
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

    static HeadImplicitMultiplyOperatorParser head_implicit_multiply_operator_parser;
    AddParsers({ &head_implicit_multiply_operator_parser, ExpressionParser::Instance() });

    static TailImplicitMultiplyOperatorParser tail_implicit_multiply_operator_parser;
    AddParsers({ &tail_implicit_multiply_operator_parser });

    AddParsers({});
}

}