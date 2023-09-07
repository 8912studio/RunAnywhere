#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/element/header_element.h"
#include "help/markdown/parse/header_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

TEST(HeaderParserTest, Parse) {

    auto test = [](std::wstring_view input, HeaderDepth depth, ElementList expected) {
        ParseContext context(input);
        auto element = HeaderParser::Instance()->Parse(context);
        if (!element) {
            return false;
        }
        return element->IsEqualTo(*MakeHeader(depth, std::move(expected)));
    };

    ASSERT_TRUE(test(L"# header1", HeaderDepth::_1, { MakeText(L"header1") }));
    ASSERT_TRUE(test(L"## header2", HeaderDepth::_2, { MakeText(L"header2") }));
    ASSERT_TRUE(test(L"### header3", HeaderDepth::_3, { MakeText(L"header3") }));
    ASSERT_TRUE(test(
        L"  ##   header head   er  ", 
        HeaderDepth::_2, 
        { MakeText(L"header head   er") }));
    ASSERT_TRUE(test(L"## header  #  #####", HeaderDepth::_2, { MakeText(L"header  #") }));
    ASSERT_TRUE(test(L"## header  #  #   ", HeaderDepth::_2, { MakeText(L"header  #") }));
    ASSERT_TRUE(test(L"# header\n", HeaderDepth::_1, { MakeText(L"header") }));
}


TEST(HeaderParserTest, ContextIndex) {

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