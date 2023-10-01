#include "module/tool/error/error_command.h"
#include <zaf/creation.h>
#include "help/built_in_help_content_manager.h"
#include "module/common/error_view.h"
#include "module/common/command_error_control.h"
#include "module/common/copy_executor.h"
#include "utility/general_number_interpreter.h"

namespace ra::mod::tool::error {

std::wstring ErrorCommand::Keyword() {
    return L"err";
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
        command_line.Arguments().front().Content(),
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


std::wstring ErrorCommand::GetKeyword() {
    return Keyword();
}


std::shared_ptr<help::markdown::element::Element> ErrorCommand::GetHelpContent() {
    return help::BuiltInHelpContentManager::Instance().GetDetail(Keyword());
}


bool ErrorCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    //Not allow to reuse.
    if (is_reusing) {
        return false;
    }

    parse_result_ = Parse(command_line);
    return true;
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


std::shared_ptr<CommandExecutor> ErrorCommand::GetExecutor() {

    if (!preview_control_) {
        return nullptr;
    }

    return CopyExecutor::TryCreate(preview_control_->GetText());
}

}