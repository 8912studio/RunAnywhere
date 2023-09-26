#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/element/header_element.h"
#include "help/markdown/parse/header_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

TEST(HeaderParserTest, Success) {

    auto test = [](std::wstring_view input, HeaderDepth depth, ElementList expected) {
        ParseContext context(input);
        HeaderParser parser;
        auto status = parser.ParseOneLine(context);
        if (status != HeaderParser::Status::Finished) {
            return false;
        }
        auto element = parser.FinishCurrentElement();
        if (!element) {
            return false;
        }
        return element->IsEqualTo(*MakeHeader(depth, std::move(expected)));
    };

    ASSERT_TRUE(test(L"# header1", HeaderDepth::_1, { MakeText(L"header1") }));
    ASSERT_TRUE(test(L"## header2", HeaderDepth::_2, { MakeText(L"header2") }));
    ASSERT_TRUE(test(L"### header3", HeaderDepth::_3, { MakeText(L"header3") }));
    ASSERT_TRUE(test(L"#### header4", HeaderDepth::_4, { MakeText(L"header4") }));
    ASSERT_TRUE(test(L"##### header5", HeaderDepth::_5, { MakeText(L"header5") }));
    ASSERT_TRUE(test(L"###### header6", HeaderDepth::_6, { MakeText(L"header6") }));
    ASSERT_TRUE(test(
        L"  ##   header head   er  ", 
        HeaderDepth::_2, 
        { MakeText(L"header head   er") }));
    ASSERT_TRUE(test(L"## header  #  #####", HeaderDepth::_2, { MakeText(L"header  #") }));
    ASSERT_TRUE(test(L"## header  #  #   ", HeaderDepth::_2, { MakeText(L"header  #") }));
    ASSERT_TRUE(test(L"# header\n", HeaderDepth::_1, { MakeText(L"header") }));
}


TEST(HeaderParserTest, Failure) {

    auto test = [](std::wstring_view input) {
        ParseContext context(input);
        HeaderParser parser;
        auto status = parser.ParseOneLine(context);
        if (status != HeaderParser::Status::Failed) {
            return false;
        }
        return context.CurrentIndex() == 0;
    };

    ASSERT_TRUE(test(L"#"));
    ASSERT_TRUE(test(L"# "));
    ASSERT_TRUE(test(L"#   "));
    ASSERT_TRUE(test(L"#   #"));
}