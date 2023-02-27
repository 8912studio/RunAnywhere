#include "module/tool/error/error_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"
#include "utility/number_parsing.h"

namespace ra::module::tool::error {

CommandBrief ErrorCommand::GetBrief() {
    return CommandBrief{
        L"err",
        L"Show error message"
    };
}


std::optional<ErrorCommandParseResult> ErrorCommand::Parse(
    const utility::CommandLine& command_line) {

    if (command_line.Arguments().empty()) {
        return std::nullopt;
    }

    calculator::EvaluateResult parse_result;
    auto parse_status = utility::ParseNumber(command_line.Arguments().front(), parse_result);
    if (parse_status == utility::NumberParseStatus::NotNumber) {
        return std::nullopt;
    }

    ErrorCommandParseResult result;
    if (parse_status == utility::NumberParseStatus::OK) {
        result.error_code = parse_result.decimal_value.convert_to<std::uint32_t>();
    }
    return result;
}


ErrorCommand::ErrorCommand(const utility::CommandLine& command_line) :
    parse_result_(Parse(command_line)) {

}


std::shared_ptr<CommandPreviewControl> ErrorCommand::GetPreviewControl() {

    preview_control_ = zaf::Create<ErrorPreviewControl>(parse_result_);
    return preview_control_;
}


void ErrorCommand::Execute() {

    std::wstring text;
    if (preview_control_) {
        text = preview_control_->GetText();
    }

    if (!text.empty()) {
        utility::SetStringToClipboard(text);
    }
}

}