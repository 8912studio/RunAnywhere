#include <gtest/gtest.h>
#include "module/calculator/parse/command_parser.h"

using namespace ra::module::calculator;

TEST(CommandParserTest, Success) {

    auto test = [](const std::wstring& input) {

        ParseContext parse_context{ input };
        ParseResult parse_result;

        CommandParser::Instance()->Parse(parse_context, parse_result);
    };
}