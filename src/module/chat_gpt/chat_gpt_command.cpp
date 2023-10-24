#include "module/chat_gpt/chat_gpt_command.h"
#include <zaf/creation.h>
#include "module/chat_gpt/chat_gpt_command_parsing.h"
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

    InitializePreviewControl();
    InitializeExecutor();

    preview_control_->ShowQuestion(parse_result->question);
    executor_->SetQuestion(parse_result->question);
    return true;
}


void ChatGPTCommand::InitializePreviewControl() {

    if (preview_control_) {
        return;
    }

    preview_control_ = zaf::Create<ChatGPTPreviewControl>();
}


void ChatGPTCommand::InitializeExecutor() {

    if (executor_) {
        return;
    }

    executor_ = zaf::Create<ChatGPTExecutor>(client_);

    zaf::Subject<std::wstring> map_subject;

    auto map_observable = map_subject.AsObservable();
    Subscriptions() += executor_->BeginEvent().Subscribe([this, map_observable](zaf::None) {
        preview_control_->ShowAnswer(map_observable);
    });

    auto map_observer = map_subject.AsObserver();
    Subscriptions() += executor_->FinishEvent().Subscribe(
        [map_observer](const comm::ChatCompletion& completion) {
            map_observer.OnNext(completion.Message().Content());
        },
        [map_observer](const zaf::Error& error) {
            map_observer.OnError(error);
        }, 
        [map_observer]() {
            map_observer.OnCompleted();
        });
}


std::shared_ptr<CommandPreviewControl> ChatGPTCommand::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> ChatGPTCommand::GetExecutor() {
    return executor_;
}

}