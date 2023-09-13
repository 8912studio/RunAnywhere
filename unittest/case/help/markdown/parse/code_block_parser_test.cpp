#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/code_block_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

TEST(CodeBlockParserTest, Success) {

    auto test = [](std::wstring_view input, std::wstring_view expected) {
        ParseContext context(input);
        auto element = CodeBlockParser::Instance()->Parse(context);
        if (!element) {
            return false;
        }
        return element->IsEqualTo(*MakeCodeBlock(std::wstring(expected)));
    };

    ASSERT_TRUE(test(L"```\n```", L""));
    ASSERT_TRUE(test(L"```\n\n```", L""));
    ASSERT_TRUE(test(L"```\n\n\n```", L"\n"));
    ASSERT_TRUE(test(L"```\nabc\n```", L"abc"));
    ASSERT_TRUE(test(L"```\nabc \n    define\n```", L"abc \n    define"));
    ASSERT_TRUE(test(L"````\nabcd\n````", L"abcd"));
    ASSERT_TRUE(test(L"````\nabcd\n```````", L"abcd"));
    ASSERT_TRUE(test(L"```\nline1\nline2\nline3", L"line1\nline2\nline3"));
    ASSERT_TRUE(test(L"```\n``\n```", L"``"));
}


TEST(CodeBlockParserTest, ContextIndex) {

    ParseContext context(L"```\ncode block\n```\nParagraph.");
    auto element = CodeBlockParser::Instance()->Parse(context);
    ASSERT_NE(element, nullptr);
    ASSERT_TRUE(element->IsEqualTo(*MakeCodeBlock(L"code block")));
    ASSERT_EQ(context.CurrentIndex(), 19);
}


TEST(CodeBlockParserTest, Failure) {

    auto test = [](std::wstring_view input) {
        ParseContext context(input);
        auto element = CodeBlockParser::Instance()->Parse(context);
        if (element) {
            return false;
        }
        return context.CurrentIndex() == 0;
    };

    ASSERT_TRUE(test(L"```cpp`\nabcd\n```"));
}