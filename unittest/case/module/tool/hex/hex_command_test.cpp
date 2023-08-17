#include <format>
#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include <zaf/base/string/encoding_conversion.h>
#include "module/tool/hex/hex_command.h"
#include "module/tool/hex/hex_preview_control.h"

using namespace ra::context;
using namespace ra::mod;
using namespace ra::mod::tool::hex;
using namespace ra::utility;

namespace {

std::wstring GetDefaultActivePath() {
    return std::filesystem::path(__FILEW__).parent_path() / "hex_test_file3.txt";
}

std::wstring GetTestFilePath() {
    return std::filesystem::path(__FILEW__).parent_path() / "hex_test_file4.txt";
}

std::vector<std::byte> ToBinary(std::string_view string) {
    std::vector<std::byte> result;
    result.resize(string.size());
    std::memcpy(&result[0], string.data(), string.size());
    return result;
}

std::vector<std::byte> ToBinary(std::wstring_view string) {
    std::vector<std::byte> result;
    auto copied_size = string.size() * sizeof(wchar_t);
    result.resize(copied_size);
    std::memcpy(&result[0], string.data(), copied_size);
    return result;
}

std::vector<std::byte> GetTestFileBinary() {
    return ToBinary("abcd");
}

}

TEST(HexCommandTest, ShowContent) {

    auto text = [](
        const CommandLine& command_line,
        const std::vector<std::byte>& expected_binary) {

        DesktopContext context;
        context.active_path = ActivePath(GetDefaultActivePath());

        HexCommand hex_command;
        if (!hex_command.Interpret(command_line, context, false)) {
            return false;
        }

        auto preview_control = zaf::As<HexPreviewControl>(hex_command.GetPreviewControl());
        if (!preview_control) {
            return false;
        }

        return preview_control->GetBinary() == expected_binary;
    };

    //Show active path content.
    {
        std::vector<std::byte> expected{
            std::byte('1'),
            std::byte('2'),
            std::byte('3'),
            std::byte('4')
        };
        ASSERT_TRUE(text(CommandLine(L"hex"), expected));
    }

    //Use active path overriding to specified a file.
    {
        ASSERT_TRUE(text(
            CommandLine(std::format(L"hex @={}", GetTestFilePath())),
            GetTestFileBinary()));
    }

    //Use /f switch to specified a file.
    {
        ASSERT_TRUE(text(
            CommandLine(std::format(L"hex {} /f", GetTestFilePath())), 
            GetTestFileBinary()));
    }

    //Use text block and /f to specified a file.
    {
        CommandLine command_line(L"hex \ufffc /f", [](int) {
            return CommandLinePiece{ CommandLinePieceType::TextBlock, GetTestFilePath() };
        });
        ASSERT_TRUE(text(command_line, GetTestFileBinary()));
    }

    //Show text content.
    {
        ASSERT_TRUE(text(CommandLine(L"hex text"), ToBinary("text")));
    }

    //Show text block
    {
        CommandLine command_line(L"hex \ufffc", [](int) {
            return CommandLinePiece{ CommandLinePieceType::TextBlock, L"text block" };
        });
        ASSERT_TRUE(text(command_line, ToBinary("text block")));
    }

    //Show active path as text using /u8 switch.
    {
        ASSERT_TRUE(text(
            CommandLine(L"hex /u8"), 
            ToBinary(zaf::ToUTF8String(GetDefaultActivePath()))));
    }

    //Show active path as text using /u16 switch.
    {
        ASSERT_TRUE(text(CommandLine(L"hex /u16"), ToBinary(GetDefaultActivePath())));
    }
}