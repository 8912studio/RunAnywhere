#include "module/chat_gpt/chat_gpt_executor.h"
#include <zaf/base/error/check.h>
#include "module/chat_gpt/local_error.h"
#include "option/option_storage.h"

namespace ra::mod::chat_gpt {

ChatGPTExecutor::ChatGPTExecutor(
    std::shared_ptr<Conversation> conversation,
    std::shared_ptr<comm::OpenAIClient> client) 
    :
    conversation_(std::move(conversation)),
    client_(std::move(client)) {

}


void ChatGPTExecutor::SetQuestion(std::wstring question) {
    question_ = std::move(question);
}


ExecuteResult ChatGPTExecutor::Execute() {

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

    conversation_->AddMessage(Message{ std::move(question_) });

    Subscriptions() += client_->CreateChatCompletion(*conversation_)
        .Subscribe(finish_event_.AsObserver());
}

}