#include <gtest/gtest.h>
#include "module/chat_gpt/chat_gpt_command_parsing.h"

using namespace ra::mod::chat_gpt;
using namespace ra::utility;

TEST(ChatGPTCommandParsingTest, ParseSuccess) {

    auto test = [](std::wstring input, std::wstring expected) {
        auto result = ParseChatGPTCommand(CommandLine{ input });
        return result.has_value() && result->question == expected;
    };

    ASSERT_TRUE(test(L">", L""));
    ASSERT_TRUE(test(L">   ", L""));
    ASSERT_TRUE(test(L">hi", L"hi"));
    ASSERT_TRUE(test(L"> hi ", L"hi"));
    ASSERT_TRUE(test(L"> how are you today ", L"how are you today"));
    ASSERT_TRUE(test(L" > how ", L"how"));

    ASSERT_TRUE(test(
        LR"(> what does "cerebral palsy" mean)", 
        LR"(what does "cerebral palsy" mean)"));
}


TEST(ChatGPTCommandParsingTest, ParseFail) {

    auto result = ParseChatGPTCommand(CommandLine{ L"" });
    ASSERT_FALSE(result.has_value());

    result = ParseChatGPTCommand(CommandLine{ L"hi" });
    ASSERT_FALSE(result.has_value());
}