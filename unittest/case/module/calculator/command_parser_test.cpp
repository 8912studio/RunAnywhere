#include <gtest/gtest.h>
#include "module/calculator/parse/command_parser.h"

using namespace ra::mod::calculator;

TEST(CommandParserTest, Success) {

    auto test = [](const std::wstring& input) {

        ParseContext parse_context{ input };
        ParseResult parse_result;

        CommandParser::Instance()->Parse(parse_context, parse_result);
    };
}


TEST(CommandParserTest, WideChar) {

    //Make sure there is no assertion on non-ascii characters.
    ParseContext parse_context{ L"1 + ¼ÆËãÆ÷" };
    ParseResult parse_result;
    auto parse_status = CommandParser::Instance()->Parse(parse_context, parse_result);
    ASSERT_EQ(parse_status, ParseStatus::Error);
}