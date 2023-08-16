#include <gtest/gtest.h>
#include "module/tool/hex/hex_command_parsing.h"

using namespace ra::mod;
using namespace ra::mod::tool::hex;
using namespace ra::utility;

TEST(HexCommandParsingTest, ParsePosition) {

    auto result = ParseHexCommand(CommandLine{ L"hex `" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = ParseHexCommand(CommandLine{ L"hex `0" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = ParseHexCommand(CommandLine{ L"hex `100" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 100);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = ParseHexCommand(CommandLine{ L"hex `0x83" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0x83);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = ParseHexCommand(CommandLine{ L"hex `abc" });
    ASSERT_FALSE(result.has_value());
}


TEST(HexCommandParsingTest, ParseLength) {

    auto result = ParseHexCommand(CommandLine{ L"hex ~" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = ParseHexCommand(CommandLine{ L"hex ~10" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, 10);

    result = ParseHexCommand(CommandLine{ L"hex ~x40" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, 0x40);

    result = ParseHexCommand(CommandLine{ L"hex ~~" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::MaxLength);

    result = ParseHexCommand(CommandLine{ L"hex ~~8" });
    ASSERT_FALSE(result.has_value());
}


TEST(HexCommandParsingTest, ParseIncompleteHexNumber) {

    auto result = ParseHexCommand(CommandLine{ L"hex x ~0x" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);
}


TEST(HexCommandParsingTest, ParseActivePathOption) {

    CommandLine command_line(L"hex @..n");
    auto result = ParseHexCommand(command_line);
    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->general_option.active_path_option.has_value());
    ASSERT_EQ(result->general_option.active_path_option->backward_level, 2);
    ASSERT_TRUE(result->general_option.active_path_option->use_name);
    ASSERT_FALSE(result->general_option.active_path_option->overriding_path.has_value());
    ASSERT_FALSE(result->general_option.active_path_option->use_workspace_path);
}


TEST(HexCommandParsingTest, Parse) {

    auto result = ParseHexCommand(CommandLine{ L"hex `70 ~120" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 70);
    ASSERT_EQ(result->length, 120);
    ASSERT_TRUE(result->general_option.text.empty());
    ASSERT_FALSE(result->general_option.treat_text_as_file);
    ASSERT_FALSE(result->general_option.text_encoding);
    ASSERT_FALSE(result->general_option.active_path_option.has_value());

    result = ParseHexCommand(CommandLine{ L"hex ~70 `120" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 120);
    ASSERT_EQ(result->length, 70);
    ASSERT_TRUE(result->general_option.text.empty());
    ASSERT_FALSE(result->general_option.treat_text_as_file);
    ASSERT_FALSE(result->general_option.text_encoding);
    ASSERT_FALSE(result->general_option.active_path_option.has_value());

    result = ParseHexCommand(CommandLine(L"hex utf-8 /u8"));
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);
    ASSERT_EQ(result->general_option.text, L"utf-8");
    ASSERT_EQ(result->general_option.text_encoding, TextEncoding::UTF8);
    ASSERT_FALSE(result->general_option.treat_text_as_file);
    ASSERT_FALSE(result->general_option.active_path_option.has_value());

    result = ParseHexCommand(CommandLine(L"hex utf-16 /u16"));
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);
    ASSERT_EQ(result->general_option.text, L"utf-16");
    ASSERT_EQ(result->general_option.text_encoding, TextEncoding::UTF16);
    ASSERT_FALSE(result->general_option.treat_text_as_file);
    ASSERT_FALSE(result->general_option.active_path_option.has_value());

    result = ParseHexCommand(CommandLine(L"hex abcdefag /f"));
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);
    ASSERT_EQ(result->general_option.text, L"abcdefag");
    ASSERT_TRUE(result->general_option.treat_text_as_file);
    ASSERT_FALSE(result->general_option.text_encoding);
    ASSERT_FALSE(result->general_option.active_path_option.has_value());

    {
        CommandLine command_line(L"hex \ufffc", [](int) {
            return CommandLinePiece(CommandLinePieceType::TextBlock, L"/f");
        });
        result = ParseHexCommand(command_line);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->position, 0);
        ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);
        ASSERT_EQ(result->general_option.text, L"/f");
        ASSERT_FALSE(result->general_option.treat_text_as_file);
        ASSERT_FALSE(result->general_option.text_encoding);
        ASSERT_FALSE(result->general_option.active_path_option);
    }
}
