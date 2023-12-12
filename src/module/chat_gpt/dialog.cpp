#include "module/chat_gpt/dialog.h"
#include <zaf/base/error/check.h>
#include "module/chat_gpt/local_error.h"

namespace ra::mod::chat_gpt {

Dialog::Dialog(std::shared_ptr<comm::OpenAIClient> client) : client_(std::move(client)) {

}


zaf::Observable<comm::ChatCompletion> Dialog::Chat(std::wstring content) {

    ZAF_EXPECT(!ongoing_message_.has_value());

    ongoing_message_.emplace(std::move(content));

    std::vector<const Message*> sent_messages;
    sent_messages.reserve(history_messages_.size() + 1);

    for (const auto& each_message : history_messages_) {
        sent_messages.push_back(&each_message);
    }
    sent_messages.push_back(&*ongoing_message_);

    return client_->CreateChatCompletion(sent_messages)
        .Do([this](const comm::ChatCompletion& result) {

            history_messages_.push_back(std::move(*ongoing_message_));
            history_messages_.push_back(result.Message());
        },
        [](const zaf::Error& error) {
    
        })
        .Finally([this]() {
            ongoing_message_.reset();
        });
}

}