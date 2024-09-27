#include "module/ai/gpt/gpt_command.h"
#include <zaf/creation.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/gpt_command_parsing.h"
#include "utility/markdown/element/factory.h"

namespace ra::mod::ai::gpt {

GPTCommand::GPTCommand(std::shared_ptr<GPTModuleContext> context) {

    preview_control_ = zaf::Create<GPTPreviewControl>();
    gpt_executor_ = zaf::Create<GPTExecutor>(std::move(context));
}


std::wstring GPTCommand::GetKeyword() {
    return std::wstring();
}


help::HelpContent GPTCommand::GetHelpContent() {
    return help::HelpContent{
        L"",
        utility::markdown::element::MakeRoot({})
    };
}


bool GPTCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    auto parse_result = ParseGPTCommand(command_line);
    if (!parse_result) {
        return false;
    }

    preview_control_->ShowQuestion(parse_result->question);
    gpt_executor_->SetQuestion(std::move(parse_result->question));
    return true;
}


std::shared_ptr<CommandPreviewControl> GPTCommand::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> GPTCommand::GetExecutor() {
    return gpt_executor_;
}

}