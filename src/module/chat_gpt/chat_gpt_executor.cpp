#include "module/chat_gpt/chat_gpt_executor.h"

namespace ra::mod::chat_gpt {

ChatGPTExecutor::ChatGPTExecutor(std::shared_ptr<comm::OpenAIClient> client) :
    client_(std::move(client)) {

}


void ChatGPTExecutor::SetQuestion(std::wstring question) {
    question_ = std::move(question);
}


ExecuteResult ChatGPTExecutor::Execute() {

    Message message{ std::move(question_) };
    client_->CreateChatCompletion({ std::move(message) });
    return PostExecuteAction::Preserve;
}

}