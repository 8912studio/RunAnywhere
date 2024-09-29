#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include "module/ai/gpt/network/chat_result.h"

namespace ra::mod::ai::gpt {

class OpenAIClientImpl;

/**
A module that communicates with OpenAI server via network.
*/
class OpenAIClient : zaf::NonCopyableNonMovable {
public:
    OpenAIClient();
    ~OpenAIClient();

    zaf::Observable<ChatResult> CreateChatCompletion(
        const std::vector<Message>& messages);

private:
    static zaf::Observable<ChatCompletion> CreateMockChatCompletion();

    static std::string CreateRequestBody(const std::vector<Message>& messages);

private:
    std::wstring api_key_;

    std::unique_ptr<OpenAIClientImpl> impl_;
};

}