#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "help/markdown/element/header_element.h"
#include "help/markdown/parse/header_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

TEST(HeaderParserTest, Parse) {

    {
        ParseContext context(L"# header1");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_NE(element, nullptr);
        ASSERT_EQ(element->Depth(), HeaderDepth::_1);
        ASSERT_EQ(element->Children().size(), 1);
        ASSERT_TRUE(element->Children().front()->IsTextElement());
        ASSERT_EQ(element->Children().front()->Text(), L"header1");
    }

    {
        ParseContext context(L"## header2");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_NE(element, nullptr);
        ASSERT_EQ(element->Depth(), HeaderDepth::_2);
        ASSERT_EQ(element->Children().size(), 1);
        ASSERT_TRUE(element->Children().front()->IsTextElement());
        ASSERT_EQ(element->Children().front()->Text(), L"header2");
    }

    {
        ParseContext context(L"### header3");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_NE(element, nullptr);
        ASSERT_EQ(element->Depth(), HeaderDepth::_3);
        ASSERT_EQ(element->Children().size(), 1);
        ASSERT_TRUE(element->Children().front()->IsTextElement());
        ASSERT_EQ(element->Children().front()->Text(), L"header3");
    }

    {
        ParseContext context(L"  ##   header head   er  ");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_NE(element, nullptr);
        ASSERT_EQ(element->Depth(), HeaderDepth::_2);
        ASSERT_EQ(element->Children().size(), 1);
        ASSERT_TRUE(element->Children().front()->IsTextElement());
        ASSERT_EQ(element->Children().front()->Text(), L"header head   er");
    }

    {
        ParseContext context(L"## header  #  #####");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_NE(element, nullptr);
        ASSERT_EQ(element->Depth(), HeaderDepth::_2);
        ASSERT_EQ(element->Children().size(), 1);
        ASSERT_TRUE(element->Children().front()->IsTextElement());
        ASSERT_EQ(element->Children().front()->Text(), L"header  #");
    }

    {
        ParseContext context(L"## header  #  #   ");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_NE(element, nullptr);
        ASSERT_EQ(element->Depth(), HeaderDepth::_2);
        ASSERT_EQ(element->Children().size(), 1);
        ASSERT_TRUE(element->Children().front()->IsTextElement());
        ASSERT_EQ(element->Children().front()->Text(), L"header  #");
    }

    {
        ParseContext context(L"# header\n");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_NE(element, nullptr);
        ASSERT_EQ(element->Depth(), HeaderDepth::_1);
        ASSERT_EQ(element->Children().size(), 1);
        ASSERT_TRUE(element->Children().front()->IsTextElement());
        ASSERT_EQ(element->Children().front()->Text(), L"header");
    }

    {
        ParseContext context(L"#");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.GetCurrentIndex(), 0);
    }

    {
        ParseContext context(L"# ");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.GetCurrentIndex(), 0);
    }

    {
        ParseContext context(L"#   ");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.GetCurrentIndex(), 0);
    }

    {
        ParseContext context(L"#   #");
        auto element = zaf::As<HeaderElement>(HeaderParser::Instance()->Parse(context));
        ASSERT_EQ(element, nullptr);
        ASSERT_EQ(context.GetCurrentIndex(), 0);
    }
}