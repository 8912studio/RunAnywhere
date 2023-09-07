#include <gtest/gtest.h>
#include <zaf/base/range.h>
#include "help/markdown/parse/paragraph_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

TEST(ParagraphParserTest, Failure) {

    ParseContext context(L"");
    auto element = ParagraphParser::Instance()->Parse(context);
    ASSERT_EQ(element, nullptr);
}


TEST(ParagraphParserTest, ParseTextOnly) {

    auto test = [](std::wstring_view input, const std::vector<std::wstring_view>& expected) {
        ParseContext context(input);
        auto element = ParagraphParser::Instance()->Parse(context);
        if (!element) {
            return false;
        }
        if (element->Type() != ElementType::Paragraph) {
            return false;
        }
        if (element->Children().size() != expected.size()) {
            return false;
        }
        for (auto index : zaf::Range(0, expected.size())) {
            if (!element->Children()[index]->IsTextElement()) {
                return false;
            }
            if (element->Children()[index]->Text() != expected[index]) {
                return false;
            }
        }
        return true;
    };

    ASSERT_TRUE(test(L"single line paragraph", { L"single line paragraph" }));
    ASSERT_TRUE(test(L"line1\nline2", { L"line1 line2" }));
    ASSERT_TRUE(test(L"line1\nline2\n", { L"line1 line2" }));
    ASSERT_TRUE(test(L"line1 \nline2", { L"line1 line2" }));
    ASSERT_TRUE(test(L"line1 \n line2", { L"line1 line2" }));
}