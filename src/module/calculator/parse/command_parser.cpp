#include "module/calculator/parse/command_parser.h"
#include <cctype>
#include "module/calculator/parse/expression_parser.h"
#include "module/calculator/parse/modifier_parser.h"

namespace calculator {

CommandParser* CommandParser::Instance() {

    static CommandParser instance;
    return &instance;
}


void CommandParser::InitializeParsers() {

    AddParsers({
        ExpressionParser::Instance(),
        ModifierParser::Instance(),
    });
}


ParseStatus CommandParser::Parse(ParseContext& context, ParseResult& result) {

    auto status = __super::Parse(context, result);
    if (status != ParseStatus::Ok) {
        return status;
    }

    //The whole command must be parsed.
    if (context.GetCurrentIndex() != context.GetLength()) {
        return ParseStatus::Error;
    }

    return ParseStatus::Ok;
}

}