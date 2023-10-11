#pragma once

#include <zaf/rx/subject.h>
#include "module/chat_gpt/message.h"

namespace ra::mod::chat_gpt::comm {

class ChatCompletion {
public:
    explicit ChatCompletion(Message message) : message_(std::move(message)) {
    
    }

    const Message& Message() const {
        return message_;
    }

private:
    chat_gpt::Message message_;
};

}