#pragma once

#include "module/calculator/parse/terminal_parser.h"

namespace ra::module::calculator {

class UnaryOperatorParser : public TerminalParser {
public:
    //A default unary operator parser that supports all unary operators.
    static UnaryOperatorParser* Default();

    //Create an unary operator that supports only specified unary operators.
    static std::unique_ptr<UnaryOperatorParser> Create(
        const std::vector<OperatorNode::Type>& supports_unary_operator);

public:
    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override;

private:
    class UnaryOperator {
    public:
        OperatorNode::Type type{ OperatorNode::Type::None };
        wchar_t charactor{};
    };

private:
    static const std::vector<UnaryOperator>& AllUnaryOperators();

private:
    explicit UnaryOperatorParser(const std::vector<UnaryOperator>& supported_unary_operators);

private:
    std::vector<UnaryOperator> supported_unary_operators_;
};

}