#include <gtest/gtest.h>
#include "module/tool/hash/hash_command_parsing.h"
#include "utility/command_line.h"

using namespace ra::mod;
using namespace ra::mod::tool::hash;
using namespace ra::utility;

TEST(HashCommandParsingTest, Parse) {

    {
        CommandLine command_line(L"md5");
        auto result = ParseHashCommand(command_line);
        ASSERT_FALSE(result.general_option.text_encoding.has_value());
        ASSERT_EQ(result.general_option.text, std::wstring{});
        ASSERT_FALSE(result.general_option.treat_text_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.general_option.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 \"");
        auto result = ParseHashCommand(command_line);
        ASSERT_FALSE(result.general_option.text_encoding.has_value());
        ASSERT_EQ(result.general_option.text, std::wstring{});
        ASSERT_FALSE(result.general_option.treat_text_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.general_option.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 \"what is this\"");
        auto result = ParseHashCommand(command_line);
        ASSERT_FALSE(result.general_option.text_encoding.has_value());
        ASSERT_EQ(result.general_option.text, L"what is this");
        ASSERT_FALSE(result.general_option.treat_text_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.general_option.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 help /u8");
        auto result = ParseHashCommand(command_line);
        ASSERT_EQ(result.general_option.text_encoding, TextEncoding::UTF8);
        ASSERT_EQ(result.general_option.text, L"help");
        ASSERT_FALSE(result.general_option.treat_text_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.general_option.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 help /u16");
        auto result = ParseHashCommand(command_line);
        ASSERT_EQ(result.general_option.text_encoding, TextEncoding::UTF16);
        ASSERT_EQ(result.general_option.text, L"help");
        ASSERT_FALSE(result.general_option.treat_text_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.general_option.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 help /c");
        auto result = ParseHashCommand(command_line);
        ASSERT_FALSE(result.general_option.text_encoding.has_value());
        ASSERT_EQ(result.general_option.text, L"help");
        ASSERT_FALSE(result.general_option.treat_text_as_file);
        ASSERT_TRUE(result.use_uppercase);
        ASSERT_FALSE(result.general_option.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 C:\\Windows\\System32 /f /c");
        auto result = ParseHashCommand(command_line);
        ASSERT_FALSE(result.general_option.text_encoding.has_value());
        ASSERT_EQ(result.general_option.text, L"C:\\Windows\\System32");
        ASSERT_TRUE(result.general_option.treat_text_as_file);
        ASSERT_TRUE(result.use_uppercase);
        ASSERT_FALSE(result.general_option.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 \ufffc", [](int) {
            return CommandLinePiece{ CommandLinePieceType::TextBlock, L"/c" };
        });
        auto result = ParseHashCommand(command_line);
        ASSERT_FALSE(result.general_option.text_encoding.has_value());
        ASSERT_EQ(result.general_option.text, L"/c");
        ASSERT_FALSE(result.general_option.treat_text_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.general_option.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 @=C:\\Windows\\notepad.exe");
        auto result = ParseHashCommand(command_line);
        ASSERT_FALSE(result.general_option.text_encoding.has_value());
        ASSERT_TRUE(result.general_option.text.empty());
        ASSERT_FALSE(result.general_option.treat_text_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_TRUE(result.general_option.active_path_option.has_value());
        ASSERT_EQ(result.general_option.active_path_option->overriding_path, L"C:\\Windows\\notepad.exe");
    }
}