#include "module/ai/gpt/network/response_parsing.h"
#include <boost/json.hpp>
#include <zaf/base/string/encoding_conversion.h>

namespace ra::mod::ai::gpt {
namespace {

Message ParseMessage(const boost::json::value& json_root) {

    const auto& json_choice_array = json_root.at("choices").as_array();
    const auto& json_choice = json_choice_array.at(0).as_object();

    const auto& json_message = json_choice.at("message").as_object();
    const auto& json_role = json_message.at("role");
    const auto& json_content = json_message.at("content");

    auto role = boost::json::value_to<std::string>(json_role);
    auto content = boost::json::value_to<std::string>(json_content);

    return Message{ zaf::FromUTF8String(role), zaf::FromUTF8String(content) };
}


std::optional<TokenUsage> ParseTokenUsage(const boost::json::value& json_root) {

    try {

        const auto& json_usage = json_root.at("usage").as_object();

        TokenUsage result;
        result.prompt_tokens = boost::json::value_to<std::size_t>(json_usage.at("prompt_tokens"));
        result.completion_tokens =
            boost::json::value_to<std::size_t>(json_usage.at("completion_tokens"));
        result.total_tokens = boost::json::value_to<std::size_t>(json_usage.at("total_tokens"));
        return result;
    }
    catch (...) {
        return std::nullopt;
    }
}

}

std::optional<ChatCompletion> ParseChatCompletion(const std::string& response) noexcept {

    try {

        auto json_root = boost::json::parse(response);

        auto message = ParseMessage(json_root);
        auto token_usage = ParseTokenUsage(json_root);

        return ChatCompletion(std::move(message), token_usage);
    }
    catch (const std::exception&) {
        return std::nullopt;
    }
}


std::string ParseErrorMessage(const std::string& response) noexcept {

    try {
        auto json_root = boost::json::parse(response);
        const auto& error = json_root.at("error").as_object();
        const auto& message = error.at("message");
        return boost::json::value_to<std::string>(message);
    }
    catch (const std::exception&) {
        return {};
    }
}

}