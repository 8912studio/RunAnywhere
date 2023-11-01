#include <gtest/gtest.h>
#include "module/chat_gpt/chat_gpt_command_parsing.h"

using namespace ra::mod::chat_gpt;
using namespace ra::utility;

TEST(ChatGPTCommandParsingTest, ParseSuccess) {

    auto test = [](const CommandLine& command_line, std::wstring_view expected) {
        auto result = ParseChatGPTCommand(command_line);
        return result.has_value() && result->question == expected;
    };

    ASSERT_TRUE(test(CommandLine{ L">" }, L""));
    ASSERT_TRUE(test(CommandLine{ L">   " }, L""));
    ASSERT_TRUE(test(CommandLine{ L">hi" }, L"hi"));
    ASSERT_TRUE(test(CommandLine{ L"> hi " }, L"hi"));
    ASSERT_TRUE(test(CommandLine{ L"> how are you today " }, L"how are you today"));
    ASSERT_TRUE(test(CommandLine{ L" > how " }, L"how"));

    ASSERT_TRUE(test(
        CommandLine{ LR"(> what does "cerebral palsy" mean)" },
        LR"(what does "cerebral palsy" mean)"));

    //Text block object
    ASSERT_TRUE(test(CommandLine{ L"> abc\ufffcdef", [](int) {
        return CommandLinePiece{ CommandLinePieceType::TextBlock, L"1123"};
    } }, L"abc 1123 def"));

    ASSERT_TRUE(test(CommandLine{ L"> abc \ufffc def", [](int) {
        return CommandLinePiece{ CommandLinePieceType::TextBlock, L"1123"};
    } }, L"abc 1123 def"));

    ASSERT_TRUE(test(CommandLine{ L">\ufffc def", [](int) {
        return CommandLinePiece{ CommandLinePieceType::TextBlock, L"aab"};
    } }, L"aab def"));

    ASSERT_TRUE(test(CommandLine{ L">\ufffc", [](int) {
        return CommandLinePiece{ CommandLinePieceType::TextBlock, L"oo"};
    } }, L"oo"));

    ASSERT_TRUE(test(CommandLine{ L">\ufffc\ufffc", [](int) {
        return CommandLinePiece{ CommandLinePieceType::TextBlock, L"obj"};
    } }, L"obj obj"));

    //Active path object
    ASSERT_TRUE(test(CommandLine{ L">abc\ufffcdef", [](int) {
        return CommandLinePiece{ CommandLinePieceType::ActivePath, L"C:\\Windows"};
    } }, L"abc def"));

    ASSERT_TRUE(test(CommandLine{ L">abc \ufffc def", [](int) {
        return CommandLinePiece{ CommandLinePieceType::ActivePath, L"C:\\Windows"};
    } }, L"abc  def"));
}


TEST(ChatGPTCommandParsingTest, ParseFail) {

    auto result = ParseChatGPTCommand(CommandLine{ L"" });
    ASSERT_FALSE(result.has_value());

    result = ParseChatGPTCommand(CommandLine{ L"hi" });
    ASSERT_FALSE(result.has_value());

    result = ParseChatGPTCommand(CommandLine{ L"hi>" });
    ASSERT_FALSE(result.has_value());

    result = ParseChatGPTCommand(CommandLine{ L"hi >" });
    ASSERT_FALSE(result.has_value());
}