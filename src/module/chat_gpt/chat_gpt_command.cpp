#include "module/chat_gpt/chat_gpt_command.h"
#include <zaf/creation.h>
#include "module/chat_gpt/chat_gpt_command_parsing.h"
#include "module/chat_gpt/local_error.h"
#include "module/common/copy_executor.h"
#include "utility/markdown/element/factory.h"

namespace ra::mod::chat_gpt {

ChatGPTCommand::ChatGPTCommand(std::shared_ptr<comm::OpenAIClient> client) : 
    client_(std::move(client)) {

    preview_control_ = zaf::Create<ChatGPTPreviewControl>();
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

    ZAF_EXPECT(command_state_ == CommandState::Waiting);

    auto parse_result = ParseChatGPTCommand(command_line);
    if (!parse_result) {
        return false;
    }

    question_ = std::move(parse_result->question);
    preview_control_->ShowQuestion(question_);
    return true;
}


std::shared_ptr<CommandPreviewControl> ChatGPTCommand::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> ChatGPTCommand::GetExecutor() {

    if (command_state_ == CommandState::Waiting) {
        CreateExecutor();
        chat_gpt_executor_->SetQuestion(question_);
        return chat_gpt_executor_;
    }
    else if (command_state_ == CommandState::Completed) {
        return CopyExecutor::TryCreate(answer_);
    }
    return nullptr;
}


void ChatGPTCommand::CreateExecutor() {

    if (chat_gpt_executor_) {
        return;
    }

    chat_gpt_executor_ = zaf::Create<ChatGPTExecutor>(client_);

    zaf::Subject<std::wstring> bridge_subject;

    auto map_observable = bridge_subject.AsObservable();
    Subscriptions() += chat_gpt_executor_->BeginEvent().Subscribe(
        [this, map_observable](zaf::None) {
            preview_control_->ShowAnswer(map_observable);
            command_state_ = CommandState::Executing;
            NotifyStateUpdated();
        });

    auto map_observer = bridge_subject.AsObserver();
    Subscriptions() += chat_gpt_executor_->FinishEvent().Do(
        [this, map_observer](const comm::ChatCompletion& completion) {
            answer_ = completion.Message().Content();
            map_observer.OnNext(answer_);
        },
        [this, map_observer](const zaf::Error& error) {
            map_observer.OnError(error);
            if (error.Code().category() == LocalCategory()) {
                command_state_ = CommandState::Failed;
            }
            else {
                command_state_ = CommandState::Waiting;
            }
        },
        [this, map_observer]() {
            map_observer.OnCompleted();
            command_state_ = CommandState::Completed;
        }
    ).Finally([this]() {
        //Destroy executor in order to re-create a new one next time.
        chat_gpt_executor_.reset();
        NotifyStateUpdated();
    })
    .Subscribe();
}

}