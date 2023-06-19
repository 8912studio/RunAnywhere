#include "module/calculator/command_parsing.h"
#include "module/calculator/parse/command_parser.h"
#include "module/calculator/parse/parse_context.h"

namespace ra::mod::calculator {
namespace {

std::wstring TryToReviseCommand(const std::wstring& command, const ParseContext& context) {

    //Split command to expression and modifier.
    std::wstring expression;
    std::wstring modifier;
    auto modifier_begin_index = command.rfind(L';');
    if (modifier_begin_index != std::wstring::npos) {
        expression = command.substr(0, modifier_begin_index);
        modifier = command.substr(modifier_begin_index);
    }
    else {
        expression = command;
    }

    //Revise only when there is error at the end of command or expression.
    if (context.GetCurrentIndex() != command.length() && 
        context.GetCurrentIndex() != expression.length()) {

        return std::wstring{};
    }

    //Revise mismatched parenthesis.
    if (context.GetParseErrorReason() == ParseErrorReason::MismatchedParenthesis) {

        auto result = expression;
        result.append(context.GetParenthesisDeep(), L')');
        result.append(modifier);
        return result;
    }

    if (context.GetLastParsedLength() <= 0) {
        return std::wstring{};
    }

    //Last token caused error, remove it.
    std::wstring result;

    if (context.GetCurrentIndex() == expression.length()) {

        result = expression.substr(0, expression.length() - context.GetLastParsedLength());
        result.append(modifier);
    }
    else {

        result = expression;
        result.append(modifier.substr(0, modifier.length() - context.GetLastParsedLength()));
    }

    return result;
}

}


std::optional<ParseResult> ParseCommand(std::wstring_view command) {

    std::wstring parsed_command{ command.data(), command.size() };
    while (!parsed_command.empty()) {

        ParseContext parse_context{ parsed_command };
        ParseResult parse_result;
        auto parse_status = CommandParser::Instance()->Parse(parse_context, parse_result);
        if (parse_status == ParseStatus::Ok) {
            return parse_result;
        }

        if (parse_status == ParseStatus::Mismatched) {
            return std::nullopt;
        }

        parsed_command = TryToReviseCommand(parsed_command, parse_context);
    }

    return std::nullopt;
}

}