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

            auto length = ParseNumber(each_argument.substr(1));
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
        L"Display file content in hex",
    };
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

}