#pragma once

#include <deque>
#include <zaf/base/non_copyable.h>
#include "module/chat_gpt/message.h"

namespace ra::mod::chat_gpt {

class Conversation : zaf::NonCopyable {
public:
    void AddMessage(Message message);

    const std::deque<Message>& Messages() const {
        return messages_;
    }

private:
    std::size_t max_message_count_{ 1 };

    //Messages in the conversation, from old to new.
    std::deque<Message> messages_;
};

}