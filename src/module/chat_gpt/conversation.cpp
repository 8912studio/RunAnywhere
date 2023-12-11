#include "module/chat_gpt/conversation.h"

namespace ra::mod::chat_gpt {

void Conversation::AddMessage(Message message) {

    messages_.push_back(std::move(message));

    if (messages_.size() > max_message_count_) {
        messages_.pop_front();
    }
}

}