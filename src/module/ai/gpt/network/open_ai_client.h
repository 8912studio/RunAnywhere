#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <vector>
#include <boost/json.hpp>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include "module/ai/gpt/network/chat_completion.h"

namespace ra::mod::ai::gpt {

class OpenAIClientImpl;

/**
A module that communicates with OpenAI server via network.
*/
class OpenAIClient : zaf::NonCopyableNonMovable {
public:
    OpenAIClient();
    ~OpenAIClient();

    zaf::Observable<ChatCompletion> CreateChatCompletion(
        const std::vector<const Message*>& messages);

private:
    static zaf::Observable<ChatCompletion> CreateMockChatCompletion();

    static std::string CreateRequestBody(const std::vector<const Message*>& messages);
    static std::optional<ChatCompletion> ParseChatCompletion(const std::string& response);
    static Message ParseMessage(const boost::json::value& json_root);
    static std::optional<TokenUsage> ParseTokenUsage(const boost::json::value& json_root);
    static std::string ParseErrorMessage(const std::string& response);

private:
    std::wstring api_key_;

    std::unique_ptr<OpenAIClientImpl> impl_;
};

}