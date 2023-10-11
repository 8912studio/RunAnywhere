#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include "module/chat_gpt/comm/chat_completion.h"
#include "module/chat_gpt/message.h"

namespace ra::mod::chat_gpt::comm {

class OpenAIClientImpl;

/**
A module that communicates with OpenAI server via network.
*/
class OpenAIClient : zaf::NonCopyableNonMovable {
public:
    OpenAIClient();
    ~OpenAIClient();

    zaf::Observable<ChatCompletion> CreateChatCompletion(const std::vector<Message>& messages);

private:
    std::wstring api_key_;

    std::unique_ptr<OpenAIClientImpl> impl_;
};

}