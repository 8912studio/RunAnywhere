#include "module/calculator/parse/unary_operator_parser.h"

namespace ra::mod::calculator {

UnaryOperatorParser* UnaryOperatorParser::Default() {
    static UnaryOperatorParser default_instance{ AllUnaryOperators() };
    return &default_instance;
}


std::unique_ptr<UnaryOperatorParser> UnaryOperatorParser::Create(
    const std::vector<OperatorNode::Type>& supports_unary_operator) {

    std::vector<UnaryOperator> operators;
    for (auto each_type : supports_unary_operator) {
        for (const auto& each_operator : AllUnaryOperators()) {
            if (each_type == each_operator.type) {
                operators.push_back(each_operator);
            }
        }
    }

    return std::unique_ptr<UnaryOperatorParser>(new UnaryOperatorParser{ operators });
}


const std::vector<UnaryOperatorParser::UnaryOperator>& UnaryOperatorParser::AllUnaryOperators() {
    static std::vector<UnaryOperator> all_operators{
        { OperatorNode::Type::Positive, L'+' },
        { OperatorNode::Type::Negative, L'-' },
        { OperatorNode::Type::Not, L'~' },
    };
    return all_operators;
}


UnaryOperatorParser::UnaryOperatorParser(
    const std::vector<UnaryOperator>& supported_unary_operators) 
    : 
    supported_unary_operators_(supported_unary_operators) {

}



ParseStatus UnaryOperatorParser::Parse(ParseContext& context, ParseResult& parse_result) {

    auto reader = context.BeginRead();

    OperatorNode::Type operator_type = OperatorNode::Type::None;

    auto ch = reader.GetChar();
    for (const auto& each_operator : supported_unary_operators_) {
        if (ch == each_operator.charactor) {
            operator_type = each_operator.type;
            break;
        }
    }

    if (operator_type == OperatorNode::Type::None) {
        return ParseStatus::Mismatched;
    }

    reader.Forward();

    auto operator_node = std::make_shared<OperatorNode>();
    operator_node->type = operator_type;
    parse_result.AddOperator(operator_node);

    return ParseStatus::Ok;
}

}