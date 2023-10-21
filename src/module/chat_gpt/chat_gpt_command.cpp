#include "module/chat_gpt/chat_gpt_command.h"
#include <zaf/creation.h>
#include "utility/markdown/element/factory.h"

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

    auto question = raw_text.substr(1);

    if (!preview_control_) {
        preview_control_ = zaf::Create<ChatGPTPreviewControl>();
    }

    InitializeExecutor();
    executor_->SetQuestion(question);

    return true;
}


void ChatGPTCommand::InitializeExecutor() {

    if (executor_) {
        return;
    }

    executor_ = zaf::Create<ChatGPTExecutor>(client_);

    Subscriptions() += executor_->FinishEvent().Subscribe([this](
    const comm::ChatCompletion& completion) {
        preview_control_->SetAnswer(completion.Message().Content());
    },
    [this](const zaf::Error& error) {
    
    });
}


std::shared_ptr<CommandPreviewControl> ChatGPTCommand::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> ChatGPTCommand::GetExecutor() {
    return executor_;
}

}