#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/inline_code_parser.h"

using namespace ra::utility::markdown::element;
using namespace ra::utility::markdown::parse;

TEST(InlineCodeParserTest, Parse) {

    auto test = [](std::wstring_view input, std::wstring_view expected) {
        ParseContext context(input);
        auto element = InlineCodeParser::Instance()->Parse(context);
        if (!element) {
            return false;
        }
        return element->IsEqualTo(*MakeInlineCode(std::wstring(expected)));
    };

    ASSERT_TRUE(test(L"`abc`", L"abc"));

    //Spaces at head and tail will be trimmed.
    ASSERT_TRUE(test(L"` abc  `", L"abc"));
    ASSERT_TRUE(test(L"`  abc`", L"abc"));
    ASSERT_TRUE(test(L"`abc   `", L"abc"));

    ASSERT_TRUE(test(L"``two backquotes``", L"two backquotes"));
    ASSERT_TRUE(test(L"```three backquotes```", L"three backquotes"));

    //Backquotes in inline code.
    ASSERT_TRUE(test(L"`` ` ``", L"`"));
    ASSERT_TRUE(test(L"`` ``` ``", L"```"));
}


TEST(InlineCodeParserTest, Failure) {

    {
        ParseContext context(L"` abc \n ` ");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.CurrentIndex(), 0);
    }

    {
        ParseContext context(L"` abc   ");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.CurrentIndex(), 0);
    }

    {
        ParseContext context(L" abc `  ");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.CurrentIndex(), 0);
    }

    {
        ParseContext context(L"``");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.CurrentIndex(), 0);
    }
}