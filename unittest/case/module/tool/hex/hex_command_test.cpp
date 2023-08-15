#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "module/tool/hex/hex_command.h"
#include "module/tool/hex/hex_preview_control.h"

using namespace ra::context;
using namespace ra::mod::tool::hex;
using namespace ra::utility;

TEST(HexCommandTest, ParsePosition) {

    auto result = HexCommand::Parse(CommandLine{ L"hex `" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = HexCommand::Parse(CommandLine{ L"hex `0" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = HexCommand::Parse(CommandLine{ L"hex `100" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 100);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = HexCommand::Parse(CommandLine{ L"hex `0x83" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0x83);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = HexCommand::Parse(CommandLine{ L"hex `abc" });
    ASSERT_FALSE(result.has_value());
}


TEST(HexCommandTest, ParseLength) {

    auto result = HexCommand::Parse(CommandLine{ L"hex ~"});
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = HexCommand::Parse(CommandLine{ L"hex ~10" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, 10);

    result = HexCommand::Parse(CommandLine{ L"hex ~x40" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, 0x40);

    result = HexCommand::Parse(CommandLine{ L"hex ~~" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::MaxLength);

    result = HexCommand::Parse(CommandLine{ L"hex ~~8"});
    ASSERT_FALSE(result.has_value());
}


TEST(HexCommandTest, ParseIncompleteHexNumber) {

    auto result = HexCommand::Parse(CommandLine{ L"hex x ~0x"});
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);
}


TEST(HexCommandTest, ParseActivePathOption) {

    CommandLine command_line(L"hex @..n");
    auto result = HexCommand::Parse(command_line);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->active_path_option.has_value());
    ASSERT_EQ(result->active_path_option->backward_level, 2);
    ASSERT_TRUE(result->active_path_option->use_name);
    ASSERT_FALSE(result->active_path_option->overriding_path.has_value());
    ASSERT_FALSE(result->active_path_option->use_workspace_path);
}


TEST(HexCommandTest, Parse) {

    auto result = HexCommand::Parse(CommandLine{ L"hex `70 ~120" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 70);
    ASSERT_EQ(result->length, 120);

    result = HexCommand::Parse(CommandLine{ L"hex ~70 `120" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 120);
    ASSERT_EQ(result->length, 70);
}


TEST(HexCommandTest, ShowFileContent) {

    std::filesystem::path current_file_path = __FILEW__;

    auto text = [&current_file_path](const CommandLine& command_line, std::wstring_view expected_file_path) {

        DesktopContext context;
        context.active_path = ActivePath(current_file_path);

        HexCommand hex_command;
        if (!hex_command.Interpret(command_line, context, false)) {
            return false;
        }

        auto preview_control = zaf::As<HexPreviewControl>(hex_command.GetPreviewControl());
        if (!preview_control) {
            return false;
        }

        return preview_control->GetFilePath() == expected_file_path;
    };

    ASSERT_TRUE(text(CommandLine(L"hex"), current_file_path.wstring()));
    ASSERT_TRUE(text(
        CommandLine(L"hex @=C:\\Windows\\explorer.exe"), 
        L"C:\\Windows\\explorer.exe"));
    ASSERT_TRUE(text(
        CommandLine(L"hex @.."),
        current_file_path.parent_path().parent_path().wstring()));
}