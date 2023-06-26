#include <gtest/gtest.h>
#include "module/tool/base64/base64_command_parsing.h"

using namespace ra::mod;
using namespace ra::mod::tool::base64;
using namespace ra::utility;

TEST(Base64CommandParsingTest, Parse) {

    auto result = Parse(CommandLine{ L"base" });
    ASSERT_EQ(result.input_text, L"");
    ASSERT_FALSE(result.operation.has_value());
    ASSERT_FALSE(result.encoding.has_value());
    ASSERT_FALSE(result.use_binary);

    result = Parse(CommandLine{ L"base 1234" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_FALSE(result.operation.has_value());
    ASSERT_FALSE(result.encoding.has_value());
    ASSERT_FALSE(result.use_binary);

    result = Parse(CommandLine{ L"base 1234 /" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_FALSE(result.operation.has_value());
    ASSERT_FALSE(result.encoding.has_value());
    ASSERT_FALSE(result.use_binary);

    result = Parse(CommandLine{ L"base 1234 /e" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Encode);
    ASSERT_FALSE(result.encoding.has_value());
    ASSERT_FALSE(result.use_binary);

    result = Parse(CommandLine{ L"base 1234 /d" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Decode);
    ASSERT_FALSE(result.encoding.has_value());
    ASSERT_FALSE(result.use_binary);

    result = Parse(CommandLine{ L"base 1234 /d /u8" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Decode); 
    ASSERT_TRUE(result.encoding.has_value());
    ASSERT_EQ(*result.encoding, TextEncoding::UTF8);
    ASSERT_FALSE(result.use_binary);

    result = Parse(CommandLine{ L"base 1234 /e /u16" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Encode);
    ASSERT_TRUE(result.encoding.has_value());
    ASSERT_EQ(*result.encoding, TextEncoding::UTF16);
    ASSERT_FALSE(result.use_binary);

    result = Parse(CommandLine{ L"base abcd /e /b" });
    ASSERT_EQ(result.input_text, L"abcd");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Encode);
    ASSERT_FALSE(result.encoding.has_value());
    ASSERT_TRUE(result.use_binary);

    result = Parse(CommandLine{ L"base abcd /d /b" });
    ASSERT_EQ(result.input_text, L"abcd");
    ASSERT_TRUE(result.operation.has_value());
    ASSERT_EQ(*result.operation, Base64Operation::Decode);
    ASSERT_FALSE(result.encoding.has_value());
    ASSERT_TRUE(result.use_binary);
}