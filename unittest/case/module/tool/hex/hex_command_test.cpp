#include <gtest/gtest.h>
#include "module/tool/hex/hex_command.h"

using namespace ra::mod::tool::hex;
using namespace ra::utility;

TEST(HexCommandTest, ParsePosition) {

    auto result = HexCommand::Parse(CommandLine{ L"hex 0" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = HexCommand::Parse(CommandLine{ L"hex 100" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 100);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = HexCommand::Parse(CommandLine{ L"hex 0x83" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0x83);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = HexCommand::Parse(CommandLine{ L"hex abc" });
    ASSERT_FALSE(result.has_value());
}


TEST(HexCommandTest, ParseLength) {

    auto result = HexCommand::Parse(CommandLine{ L"hex l"});
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);

    result = HexCommand::Parse(CommandLine{ L"hex L10" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, 10);

    result = HexCommand::Parse(CommandLine{ L"hex Lx40" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, 0x40);

    result = HexCommand::Parse(CommandLine{ L"hex LL8"});
    ASSERT_FALSE(result.has_value());
}


TEST(HexCommandTest, ParseIncompleteHexNumber) {

    auto result = HexCommand::Parse(CommandLine{ L"hex x L0x"});
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 0);
    ASSERT_EQ(result->length, HexCommandParseResult::DefaultLength);
}


TEST(HexCommandTest, Parse) {

    auto result = HexCommand::Parse(CommandLine{ L"hex 70 l120" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 70);
    ASSERT_EQ(result->length, 120);

    result = HexCommand::Parse(CommandLine{ L"hex l70 120" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->position, 120);
    ASSERT_EQ(result->length, 70);
}