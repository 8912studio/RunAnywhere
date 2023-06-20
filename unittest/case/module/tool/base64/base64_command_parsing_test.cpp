#include <gtest/gtest.h>
#include "module/tool/base64/base64_command_parsing.h"

using namespace ra::mod;
using namespace ra::mod::tool::base64;
using namespace ra::utility;

TEST(Base64CommandParsingTest, Parse) {

    auto result = Parse(CommandLine{ L"b64" });
    ASSERT_EQ(result.input_text, L"");
    ASSERT_FALSE(result.operation.has_value());
    ASSERT_EQ(result.encoding, TextEncoding::UTF8);

    result = Parse(CommandLine{ L"b64 1234" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_FALSE(result.operation.has_value());
    ASSERT_EQ(result.encoding, TextEncoding::UTF8);

    result = Parse(CommandLine{ L"b64 1234 /" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_FALSE(result.operation.has_value());
    ASSERT_EQ(result.encoding, TextEncoding::UTF8);

    result = Parse(CommandLine{ L"b64 1234 /e" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Encode);
    ASSERT_EQ(result.encoding, TextEncoding::UTF8);

    result = Parse(CommandLine{ L"b64 1234 /d" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Decode);
    ASSERT_EQ(result.encoding, TextEncoding::UTF8);

    result = Parse(CommandLine{ L"b64 1234 /d /u8" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Decode);
    ASSERT_EQ(result.encoding, TextEncoding::UTF8);

    result = Parse(CommandLine{ L"b64 1234 /e /u16" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Encode);
    ASSERT_EQ(result.encoding, TextEncoding::UTF16);
}