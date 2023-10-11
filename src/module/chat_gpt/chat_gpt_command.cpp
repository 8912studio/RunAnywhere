#include "module/chat_gpt/chat_gpt_command.h"
#include <zaf/creation.h>
#include "help/markdown/element/factory.h"
#include "module/chat_gpt/chat_gpt_executor.h"

namespace ra::mod::chat_gpt {

ChatGPTCommand::ChatGPTCommand(std::shared_ptr<comm::OpenAIClient> client) : 
    client_(std::move(client)) {

}


std::wstring ChatGPTCommand::GetKeyword() {
    return std::wstring();
}


help::HelpContent ChatGPTCommand::GetHelpContent() {
    return help::HelpContent{
        L"",
        help::markdown::element::MakeRoot({})
    };
}


bool ChatGPTCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    const auto& raw_text = command_line.RawText();
    if (raw_text.empty()) {
        return false;
    }

    if (!preview_control_) {
        preview_control_ = zaf::Create<ChatGPTPreviewControl>();
    }

    question_ = raw_text.substr(1);
    preview_control_->SetQuestion(question_);
    return true;
}


std::shared_ptr<CommandPreviewControl> ChatGPTCommand::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> ChatGPTCommand::GetExecutor() {
    return std::make_shared<ChatGPTExecutor>(client_, std::move(question_));
}

}