#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "module/tool/md5/md5_command.h"
#include "module/tool/md5/md5_preview_control.h"

using namespace ra::context;
using namespace ra::mod;
using namespace ra::mod::tool::md5;
using namespace ra::utility;

TEST(MD5CommandTest, ParseSucceeded) {

    {
        CommandLine command_line(L"md5");
        auto result = MD5Command::Parse(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, std::wstring{});
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        CommandLine command_line(L"md5 \"");
        auto result = MD5Command::Parse(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, std::wstring{});
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        CommandLine command_line(L"md5 \"what is this\"");
        auto result = MD5Command::Parse(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, L"what is this");
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        CommandLine command_line(L"md5 help /u8");
        auto result = MD5Command::Parse(command_line);
        ASSERT_EQ(result.encoding, TextEncoding::UTF8);
        ASSERT_EQ(result.string, L"help");
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        CommandLine command_line(L"md5 help /u16");
        auto result = MD5Command::Parse(command_line);
        ASSERT_EQ(result.encoding, TextEncoding::UTF16);
        ASSERT_EQ(result.string, L"help");
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_EQ(result.use_uppercase, false);
    }

    {
        CommandLine command_line(L"md5 help /c");
        auto result = MD5Command::Parse(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, L"help");
        ASSERT_FALSE(result.treat_string_as_file);
        ASSERT_EQ(result.use_uppercase, true);
    }

    {
        CommandLine command_line(L"md5 C:\\Windows\\System32 /f /c");
        auto result = MD5Command::Parse(command_line);
        ASSERT_FALSE(result.encoding.has_value());
        ASSERT_EQ(result.string, L"C:\\Windows\\System32");
        ASSERT_TRUE(result.treat_string_as_file);
        ASSERT_EQ(result.use_uppercase, true);
    }
}


TEST(MD5CommandTest, CalculateText) {

    auto test = [](const std::wstring& command_text, const std::wstring& expected_result) {

        CommandLine command_line{ command_text };

        DesktopContext context;
        context.active_path = ActivePath{ L"C:\\windows\\notepad.exe" };

        MD5Command command;
        if (!command.Interpret(command_line, context, false)) {
            return false;
        }
        auto preview_control = zaf::As<MD5PreviewControl>(command.GetPreviewControl());
        if (!preview_control) {
            return false;
        }
        return preview_control->GetText() == expected_result;
    };

    ASSERT_TRUE(test(L"md5 md5command", L"ea577b77e0c2c8d6f896f7722af7783d"));
    ASSERT_TRUE(test(L"md5 md5command /u8", L"ea577b77e0c2c8d6f896f7722af7783d"));
    ASSERT_TRUE(test(L"md5 md5command /c", L"EA577B77E0C2C8D6F896F7722AF7783D"));
    ASSERT_TRUE(test(L"md5 md5command /c /u8", L"EA577B77E0C2C8D6F896F7722AF7783D"));
    ASSERT_TRUE(test(L"md5 md5command /u16", L"98fcc00844e87906420aad87f6925ce7"));
    ASSERT_TRUE(test(L"md5 \"md5 command\"", L"1d2fa22a35318a110e4fc3b0d78da63f"));

    //Calculate active path as text.
    ASSERT_TRUE(test(L"md5 /u8", L"630e80b49e80faf5fe65ebff5e92478c"));
    ASSERT_TRUE(test(L"md5 @=D:\\Applications\\vs.exe /u16", L"568a45a037e0f6ad7e79d74ae2d34b6a"));
}


TEST(MD5CommandTest, CalculateFile) {

    auto test = [](const std::wstring& command_text, const std::wstring& expected_result) {
    

    };
}