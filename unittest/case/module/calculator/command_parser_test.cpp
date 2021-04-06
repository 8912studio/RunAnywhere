#include <gtest/gtest.h>
#include "module/calculator/parse/command_parser.h"

TEST(CommandParserTest, Success) {

    auto test = [](const std::wstring& input) {

        calculator::ParseContext parse_context{ input };
        calculator::ParseResult parse_result;

        calculator::CommandParser::Instance()->Parse(parse_context, parse_result);
    };
}