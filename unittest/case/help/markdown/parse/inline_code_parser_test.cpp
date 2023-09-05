#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "help/markdown/parse/inline_code_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

TEST(InlineCodeParser, Parse) {

    {
        ParseContext context(L"`abc`");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_NE(element, nullptr);
        ASSERT_EQ(element->Type(), ElementType::InlineCode);
        ASSERT_EQ(element->Children().size(), 1);
        ASSERT_TRUE(element->Children().front()->IsTextElement());
        ASSERT_EQ(element->Children().front()->Text(), L"abc");
    }

    {
        ParseContext context(L"` abc  `");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_NE(element, nullptr);
        ASSERT_EQ(element->Type(), ElementType::InlineCode);
        ASSERT_EQ(element->Children().size(), 1);
        ASSERT_TRUE(element->Children().front()->IsTextElement());
        ASSERT_EQ(element->Children().front()->Text(), L" abc  ");
    }

    {
        ParseContext context(L"` abc \n ` ");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.GetCurrentIndex(), 0);
    }

    {
        ParseContext context(L"` abc   ");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.GetCurrentIndex(), 0);
    }

    {
        ParseContext context(L" abc `  ");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.GetCurrentIndex(), 0);
    }

    {
        ParseContext context(L"``");
        auto element = InlineCodeParser::Instance()->Parse(context);
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.GetCurrentIndex(), 0);
    }
}