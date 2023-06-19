#include <gtest/gtest.h>
#include "module/tool/error/error_command.h"

using namespace ra::mod::tool::error;
using namespace ra::utility;

TEST(ErrorCommandTest, ParseOK) {

    auto result = ErrorCommand::Parse(CommandLine{ L"err 0" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->error_code, 0);

    result = ErrorCommand::Parse(CommandLine{ L"err 5"});
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->error_code, 5);

    result = ErrorCommand::Parse(CommandLine{ L"err 0x80070008" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->error_code, 0x80070008);

    result = ErrorCommand::Parse(CommandLine{ L"err -2147024888"});
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->error_code, -2147024888);
}


TEST(ErrorCommandTest, ParseIncomplete) {

    auto result = ErrorCommand::Parse(CommandLine{ L"err " });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->error_code, 0);

    result = ErrorCommand::Parse(CommandLine{ L"err x" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->error_code, 0);

    result = ErrorCommand::Parse(CommandLine{ L"err 0x" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->error_code, 0);

    result = ErrorCommand::Parse(CommandLine{ L"err -" });
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->error_code, 0);
}


TEST(ErrorCommandTest, ParseError) {

    auto result = ErrorCommand::Parse(CommandLine{ L"err -0x1" });
    ASSERT_FALSE(result.has_value());
}