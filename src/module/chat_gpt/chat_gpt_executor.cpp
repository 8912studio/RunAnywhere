#include "module/chat_gpt/chat_gpt_executor.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>
#include "module/chat_gpt/dialog/dialog_window.h"
#include "module/chat_gpt/local_error.h"
#include "option/option_storage.h"

namespace ra::mod::chat_gpt {

ChatGPTExecutor::ChatGPTExecutor(std::shared_ptr<Dialog> dialog) : dialog_(std::move(dialog)) {

}


void ChatGPTExecutor::SetQuestion(std::wstring question) {
    question_ = std::move(question);
}


ExecuteResult ChatGPTExecutor::Execute() {

    auto window = zaf::Create<DialogWindow>(dialog_);
    window->Show();
    window->Chat(std::move(question_));
    return PostExecuteAction::Dispose;

    //One exectuor can only execute once.
    ZAF_EXPECT(!has_executed_);
    has_executed_ = true;

    begin_event_.AsObserver().OnNext({});
    begin_event_.AsObserver().OnCompleted();

    InnerExecute();
    return PostExecuteAction::Preserve;
}


void ChatGPTExecutor::InnerExecute() {

    if (option::OptionStorage::Instance().OpenAIAPIKey().empty()) {
        finish_event_.AsObserver().OnError(zaf::Error{
            make_error_code(LocalErrc::NoAPIKey)
        });
        return;
    }

    //Not allow to send empty question.
    if (question_.empty()) {
        finish_event_.AsObserver().OnError(zaf::Error{ 
            make_error_code(LocalErrc::EmptyQuestion) 
        });
        return;
    }

    Subscriptions() += dialog_->Chat(std::move(question_)).Subscribe(finish_event_.AsObserver());
}

}