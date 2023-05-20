#include "module/tool/hex/hex_command.h"
#include <zaf/creation.h>
#include "module/calculator/evaluate/evaluator.h"
#include "module/calculator/parse/decimal_number_parser.h"
#include "module/calculator/parse/non_decimal_number_parser.h"
#include "module/common/command_error_control.h"
#include "utility/general_number_interpreter.h"

namespace ra::module::tool::hex {
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


std::optional<std::uint64_t> ParseLength(const std::wstring& input) {

    auto length_number = input.substr(1);
    if (length_number.empty()) {
        return HexCommandParseResult::DefaultLength;
    }

    if (length_number.length() == 1 && 
        std::tolower(length_number.front()) == L'l') {

        return HexCommandParseResult::MaxLength;
    }

    return ParseNumberWithDefault(length_number, HexCommandParseResult::DefaultLength);
}

}


std::optional<HexCommandParseResult> HexCommand::Parse(const utility::CommandLine& command_line) {

    HexCommandParseResult result;

    for (const auto& each_argument : command_line.Arguments()) {

        if (each_argument.empty()) {
            continue;
        }

        if (std::tolower(each_argument.front()) == L'l') {

            auto length = ParseLength(each_argument);
            if (!length) {
                return std::nullopt;
            }

            result.length = *length;
        }
        else {

            auto position = ParseNumberWithDefault(each_argument, 0);
            if (!position) {
                return std::nullopt;
            }

            result.position = *position;
        }
    }

    return result;
}


CommandBrief HexCommand::Brief() {
    return CommandBrief{
        L"hex",
        L"Display bytes of file content in hex format",
    };
}


CommandBrief HexCommand::GetBrief() {
    return Brief();
}


help::content::Content HexCommand::GetHelpContent() {

    help::content::Content result;

    result.AddTitleLine(L"hex command");
    result.AddBodyLine(L"Display bytes of active file's content in hex and ASCII format.");

    result.AddTitleLine(L"Usage");
    result.AddBodyLine(L"hex [position] [length]");
    result.AddBodyLine(L"`position` specifies the begin position of content to display. "
        "Default is 0 if it is omitted.");
    result.AddBodyLine(L"`length` specifies the length of content to display. "
        "It is prefixed with letter `l` or `L`. Default is 128 if it is omitted. "
        "The max supported length is 4096. Use two `l` or `L` letters to specify the max length, "
        "for example `ll`.");
    result.AddBodyLine(L"Both `position` and `length` can be in decimal or hexadecimal format. "
        "Use `x` or `0x` as prefix for hexadecimal.");

    return result;
}


HexCommand::HexCommand(const utility::CommandLine& command_line) : 
    parse_result_(Parse(command_line)) {

}


std::shared_ptr<CommandPreviewControl> HexCommand::GetPreviewControl() {

    if (!parse_result_) {
        return CommandErrorControl::InvalidArgument();
    }

    if (!preview_control_) {
        preview_control_ = zaf::Create<HexPreviewControl>();
        preview_control_->ShowFileContent(
            GetDesktopContext().active_path.GetPath(), 
            *parse_result_);
    }

    return preview_control_;
}


void HexCommand::Execute() {

}

}