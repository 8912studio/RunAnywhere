#include "module/ai/gpt/chat_gpt_command.h"
#include <zaf/creation.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/chat_gpt_command_parsing.h"
#include "utility/markdown/element/factory.h"

namespace ra::mod::ai::gpt {

ChatGPTCommand::ChatGPTCommand(std::shared_ptr<DialogManager> dialog_manager) {

    preview_control_ = zaf::Create<ChatGPTPreviewControl>();
    chat_gpt_executor_ = zaf::Create<ChatGPTExecutor>(std::move(dialog_manager));
}


std::wstring ChatGPTCommand::GetKeyword() {
    return std::wstring();
}


help::HelpContent ChatGPTCommand::GetHelpContent() {
    return help::HelpContent{
        L"",
        utility::markdown::element::MakeRoot({})
    };
}


bool ChatGPTCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    auto parse_result = ParseChatGPTCommand(command_line);
    if (!parse_result) {
        return false;
    }

    preview_control_->ShowQuestion(parse_result->question);
    chat_gpt_executor_->SetQuestion(std::move(parse_result->question));
    return true;
}


std::shared_ptr<CommandPreviewControl> ChatGPTCommand::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> ChatGPTCommand::GetExecutor() {
    return chat_gpt_executor_;
}

}