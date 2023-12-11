#include "module/chat_gpt/chat_gpt_command.h"
#include <zaf/creation.h>
#include <zaf/rx/creation.h>
#include <zaf/window/window.h>
#include "module/chat_gpt/answer_result.h"
#include "module/chat_gpt/chat_gpt_command_parsing.h"
#include "module/chat_gpt/local_error.h"
#include "module/common/copy_executor.h"
#include "utility/markdown/element/factory.h"

namespace ra::mod::chat_gpt {

ChatGPTCommand::ChatGPTCommand(
    std::shared_ptr<Conversation> conversation,
    std::shared_ptr<comm::OpenAIClient> client)
    : 
    conversation_(std::move(conversation)),
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

    chat_gpt_executor_ = zaf::Create<ChatGPTExecutor>(conversation_, client_);

    Subscriptions() += chat_gpt_executor_->BeginEvent().Subscribe(
        std::bind(&ChatGPTCommand::OnBeginExecute, this));
}


void ChatGPTCommand::OnBeginExecute() {

    zaf::Subject<AnswerResult> bridge_subject;

    preview_control_->ShowAnswer(bridge_subject.AsObservable());
    command_state_ = CommandState::Executing;
    NotifyStateUpdated();

    auto map_observer = bridge_subject.AsObserver();

    Subscriptions() += chat_gpt_executor_->FinishEvent()
        //Map the result to AnswerResult class.
        .Map<AnswerResult>([](const comm::ChatCompletion& completion) {
            return AnswerResult{ completion.Message().Content() };
        })
        //Map the error to AnswerResult class.
        .Catch([](const zaf::Error& error) {
            return zaf::rx::Just(AnswerResult{ error });
        })
        //If the window is being resized or moved, we can't change the size of the window, as the 
        //window manager will reset the size during resizing or moving. Hence we display the result  
        //only when the window is not being resized or moved.
        .FlatMap<AnswerResult>([this](const AnswerResult& result) {

            auto window = preview_control_->Window();
            if (!window) {
                return zaf::rx::Just(result);
            }

            return window->WhenNotSizingOrMoving().Map<AnswerResult>([result](zaf::None) {
                return result;
            });
        })
        .Subscribe([this, map_observer](const AnswerResult& result) {

            if (auto answer = result.Answer()) {
                answer_ = *answer;
                command_state_ = CommandState::Completed;
            }
            else if (auto error = result.Error()) {
                if (error->Code().category() == LocalErrorCategory()) {
                    command_state_ = CommandState::Failed;
                }
                else {
                    command_state_ = CommandState::Waiting;
                }
            }

            map_observer.OnNext(result);
            map_observer.OnCompleted();

            //Destroy executor in order to re-create a new one next time.
            chat_gpt_executor_.reset();
            NotifyStateUpdated();
        });
}

}