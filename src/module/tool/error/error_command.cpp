#include "module/tool/error/error_command.h"
#include <zaf/creation.h>
#include "module/common/error_view.h"
#include "module/common/command_error_control.h"
#include "utility/clipboard.h"
#include "utility/general_number_interpreter.h"

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
        //Use default result if there is no argument.
        return ErrorCommandParseResult{};
    }

    utility::GeneralNumberInterpreter interpreter{ true };
    calculator::EvaluateResult evaluate_result;
    auto interpret_status = interpreter.Interpret(
        command_line.Arguments().front(),
        evaluate_result);

    if (interpret_status == utility::GeneralNumberInterpreter::Status::NotNumber) {
        return std::nullopt;
    }

    ErrorCommandParseResult result;
    if (interpret_status == utility::GeneralNumberInterpreter::Status::OK) {
        result.error_code = evaluate_result.decimal_value.convert_to<std::uint32_t>();
    }
    return result;
}


ErrorCommand::ErrorCommand(const utility::CommandLine& command_line) :
    parse_result_(Parse(command_line)) {

}


help::content::Content ErrorCommand::GetHelpContent() {

    help::content::Content result;

    result.AddTitleLine(L"err command");
    result.AddBodyLine(L"Show error message for specified system error code.");

    result.AddTitleLine(L"Usage");
    result.AddBodyLine(L"err <code>");
    result.AddBodyLine(L"`code` is a value returned by GetLastError() function.");

    return result;
}


std::shared_ptr<CommandPreviewControl> ErrorCommand::GetPreviewControl() {

    if (!parse_result_) {
        return CommandErrorControl::InvalidArgument();
    }

    if (!preview_control_) {
        preview_control_ = zaf::Create<ErrorPreviewControl>();
        preview_control_->ShowErrorMessage(*parse_result_);
    }

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