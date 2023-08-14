#include <gtest/gtest.h>
#include "module/tool/md5/md5_command_parsing.h"
#include "utility/command_line.h"

using namespace ra::mod;
using namespace ra::mod::tool::md5;
using namespace ra::utility;

TEST(MD5CommandParsingTest, Parse) {

    {
        CommandLine command_line(L"md5");
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, std::wstring{});
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 \"");
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, std::wstring{});
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 \"what is this\"");
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, L"what is this");
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 help /u8");
        auto result = ParseMD5Command(command_line);
        ASSERT_EQ(result.encoding, TextEncoding::UTF8);
        ASSERT_EQ(result.string, L"help");
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 help /u16");
        auto result = ParseMD5Command(command_line);
        ASSERT_EQ(result.encoding, TextEncoding::UTF16);
        ASSERT_EQ(result.string, L"help");
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 help /c");
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, L"help");
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_TRUE(result.use_uppercase);
        ASSERT_FALSE(result.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 C:\\Windows\\System32 /f /c");
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, L"C:\\Windows\\System32");
        ASSERT_TRUE(result.treat_string_as_file);
        ASSERT_TRUE(result.use_uppercase);
        ASSERT_FALSE(result.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 \ufffc", [](int) {
            return CommandLinePiece{ CommandLinePieceType::TextBlock, L"/c" };
        });
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, L"/c");
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_FALSE(result.active_path_option.has_value());
    }

    {
        CommandLine command_line(L"md5 @=C:\\Windows\\notepad.exe");
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_TRUE(result.string.empty());
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_FALSE(result.use_uppercase);
        ASSERT_TRUE(result.active_path_option.has_value());
        ASSERT_EQ(result.active_path_option->overriding_path, L"C:\\Windows\\notepad.exe");
    }
}