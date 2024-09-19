#pragma once

#include <zaf/rx/subject.h>
#include "module/ai/gpt/dialog/message.h"

namespace ra::mod::ai::gpt {

class ChatCompletion {
public:
    explicit ChatCompletion(Message message) : message_(std::move(message)) {
    
    }

    const Message& Message() const {
        return message_;
    }

private:
    gpt::Message message_;
};

}