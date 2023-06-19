#include <gtest/gtest.h>
#include "module/tool/base64/base64_command_parsing.h"

using namespace ra::mod::tool::base64;
using namespace ra::utility;

TEST(Base64CommandParsingTest, Parse) {

    auto result = Parse(CommandLine{ L"b64" });
    ASSERT_EQ(result.input_text, L"");
    ASSERT_FALSE(result.operation.has_value());

    result = Parse(CommandLine{ L"b64 1234" });
    ASSERT_EQ(result.input_text, L"1234");
    ASSERT_FALSE(result.operation.has_value());
}