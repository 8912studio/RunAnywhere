#include "module/tool/hex/hex_command.h"
#include <zaf/creation.h>
#include "module/calculator/evaluate/evaluator.h"
#include "module/calculator/parse/decimal_number_parser.h"
#include "module/calculator/parse/non_decimal_number_parser.h"

namespace ra::module::tool::hex {
namespace {

std::shared_ptr<calculator::SyntaxNode> ParseNumberWithParser(
    const std::wstring& input,
    calculator::Parser* parser) {

    calculator::ParseContext context(input);
    calculator::ParseResult result;
    auto status = parser->Parse(context, result);
    if (status != calculator::ParseStatus::Ok) {
        return nullptr;
    }

    if (context.GetCurrentIndex() != context.GetLength()) {
        return nullptr;
    }

    return result.GetExpressionRootNode();
}


std::optional<std::uint64_t> ParseNumber(const std::wstring& input) {

    auto syntax_node = ParseNumberWithParser(input, calculator::DecimalNumberParser::Instance());
    if (!syntax_node) {

        syntax_node = ParseNumberWithParser(input, calculator::NonDecimalNumberParser::Hex());
        if (!syntax_node) {
            return std::nullopt;
        }
    }

    auto evaluator = calculator::Evaluator::Create(syntax_node);
    if (!evaluator) {
        return std::nullopt;
    }

    calculator::EvaluateResult evaluate_result;
    auto evaluate_status = evaluator->Evaluate(evaluate_result);
    if (evaluate_status != calculator::EvaluateStatus::Ok) {
        return std::nullopt;
    }

    return evaluate_result.decimal_value.convert_to<std::uint64_t>();
}

}


std::optional<HexCommandParseResult> HexCommand::Parse(const utility::CommandLine& command_line) {

    HexCommandParseResult result;

    for (const auto& each_argument : command_line.Arguments()) {

        if (each_argument.empty()) {
            continue;
        }

        if (std::tolower(each_argument.front()) == L'l') {

            auto length_number = each_argument.substr(1);
            if (length_number.empty()) {
                continue;
            }

            auto length = ParseNumber(length_number);
            if (!length) {
                return std::nullopt;
            }

            result.length = *length;
        }
        else {

            auto position = ParseNumber(each_argument);
            if (!position) {
                return std::nullopt;
            }

            result.position = *position;
        }
    }

    return result;
}


CommandBrief HexCommand::GetBrief() {
    return CommandBrief{
        L"hex",
        L"Display bytes of file content in hex format",
    };
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
        "The max supported length is 4096.");
    result.AddBodyLine(L"Both `position` and `length` can be in decimal or hexadecimal format. "
        "Use `x` or `0x` as prefix for hexadecimal.");

    return result;
}


HexCommand::HexCommand(const utility::CommandLine& command_line) : 
    parse_result_(Parse(command_line)) {

}


std::shared_ptr<CommandPreviewControl> HexCommand::GetPreviewControl() {

    if (!preview_control_) {
        preview_control_ = CreatePreviewControl();
    }

    return preview_control_;
}


std::shared_ptr<HexPreviewControl> HexCommand::CreatePreviewControl() const {

    if (!parse_result_) {
        return nullptr;
    }

    auto result = zaf::Create<HexPreviewControl>();
    result->ShowFileContent(GetDesktopContext().active_path.GetPath(), *parse_result_);
    return result;
}


void HexCommand::Execute() {

}

}