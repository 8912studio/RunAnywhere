#include "module/calculator/parse/unary_expression_parser.h"
#include "module/calculator/parse/expression_parser.h"
#include "module/calculator/parse/unary_operator_parser.h"

namespace ra::mod::calculator {

UnaryExpressionParser* UnaryExpressionParser::Instance() {

    static UnaryExpressionParser instance;
    return &instance;
}


void UnaryExpressionParser::InitializeParsers() {

    AddParsers({
        UnaryOperatorParser::Default(),
        ExpressionParser::Instance()
    });
}

}