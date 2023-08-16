#include "module/tool/hex/hex_command_parsing.h"
#include "module/active_path/active_path_option_parsing.h"
#include "module/calculator/evaluate/evaluator.h"
#include "module/calculator/parse/decimal_number_parser.h"
#include "module/calculator/parse/non_decimal_number_parser.h"
#include "module/common/general_option.h"
#include "module/common/parse_status.h"
#include "utility/general_number_interpreter.h"

namespace ra::mod::tool::hex {
namespace {

std::optional<std::uint64_t> ParseNumberWithDefault(
    const std::wstring& input,
    std::uint64_t default_value) {

    calculator::EvaluateResult evaluate_result;

    utility::GeneralNumberInterpreter interpreter{ false };
    auto interpret_status = interpreter.Interpret(input, evaluate_result);

    switch (interpret_status) {
    case utility::GeneralNumberInterpreter::Status::OK:
        return evaluate_result.decimal_value.convert_to<std::uint64_t>();;

    case utility::GeneralNumberInterpreter::Status::Incomplete:
        return default_value;

    default:
        return std::nullopt;
    }
}


std::optional<std::uint64_t> ParsePosition(const std::wstring& input) {

    if (input.empty()) {
        return 0;
    }

    return ParseNumberWithDefault(input, 0);
}


std::optional<std::uint64_t> ParseLength(const std::wstring& input) {

    if (input.empty()) {
        return HexCommandParseResult::DefaultLength;
    }

    if (input.length() == 1 && input.front() == L'~') {
        return HexCommandParseResult::MaxLength;
    }

    return ParseNumberWithDefault(input, HexCommandParseResult::DefaultLength);
}


ParseStatus ParseArgument(
    const utility::CommandLinePiece& argument, 
    HexCommandParseResult& result) {

    if (argument.Type() != utility::CommandLinePieceType::NormalText) {
        return ParseStatus::Mismatched;
    }

    const auto& text = argument.Content();
    if (text.front() == L'`') {

        auto position = ParsePosition(text.substr(1));
        if (!position) {
            return ParseStatus::Error;
        }
        result.position = *position;
        return ParseStatus::OK;
    }

    if (text.front() == L'~') {

        auto length = ParseLength(text.substr(1));
        if (!length) {
            return ParseStatus::Error;
        }
        result.length = *length;
        return ParseStatus::OK;
    }

    return ParseStatus::Mismatched;
}

}

std::optional<HexCommandParseResult> ParseHexCommand(const utility::CommandLine& command_line) {

    HexCommandParseResult result;

    for (const auto& each_argument : command_line.Arguments()) {

        auto parse_status = ParseArgument(each_argument, result);
        if (parse_status == ParseStatus::OK) {
            continue;
        }

        if (parse_status == ParseStatus::Error) {
            return std::nullopt;
        }

        ParseGeneralOption(each_argument, result.general_option);
    }

    return result;
}

}