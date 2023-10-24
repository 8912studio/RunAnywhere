#include "module/chat_gpt/chat_gpt_executor.h"

namespace ra::mod::chat_gpt {

ChatGPTExecutor::ChatGPTExecutor(std::shared_ptr<comm::OpenAIClient> client) :
    client_(std::move(client)) {

}


void ChatGPTExecutor::SetQuestion(std::wstring question) {
    question_ = std::move(question);
}


ExecuteResult ChatGPTExecutor::Execute() {

    begin_event_.AsObserver().OnNext({});
    begin_event_.AsObserver().OnCompleted();

    Message message{ std::move(question_) };

    //TODO: Need a better way to forward subscription.
    Subscriptions() += client_->CreateChatCompletion({ std::move(message) }).Subscribe(
    [this](const comm::ChatCompletion& completion) {
        finish_event_.AsObserver().OnNext(completion);
    }, 
    [this](const zaf::Error& error) {
        finish_event_.AsObserver().OnError(error);
    },
    [this]() {
        finish_event_.AsObserver().OnCompleted();
    });

    return PostExecuteAction::Preserve;
}

}