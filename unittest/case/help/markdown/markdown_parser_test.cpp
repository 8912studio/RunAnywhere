#include <gtest/gtest.h>
#include "help/markdown/markdown_parser.h"

using namespace ra::help::markdown;

namespace {

bool TestParse(std::wstring_view input, const StyledString& expected) {
    MarkdownParser parser;
    auto styled_string = parser.Parse(input);
    return styled_string == expected;
}

}


TEST(MarkdownParserTest, ParsePlainText) {

    std::wstring input = L"  line1 \n\n\n   line2  \nline3\n";

    std::wstring expected_string = L"line1\nline2\nline3\n";
    StyleSpan root_style_span(0, expected_string.length(), StyleType::Normal, {
        { 0, expected_string.length(), StyleType::Normal}
    });
    TestParse(input, StyledString(expected_string, root_style_span));
}


TEST(MarkdownParserTest, ParseHeaderType) {

    auto test = [](int hash_count) {
        std::wstring header_string = L"This is a header";

        std::wstring input(hash_count, L'#');
        input += L' ';
        input += header_string;

        StyleType style_type = StyleType(int(StyleType::Header1) + hash_count - 1);
        StyleSpan root_style_span(0, header_string.length(), StyleType::Normal, {
            { 0, header_string.length(), style_type }
        });

        return TestParse(input, StyledString(header_string, root_style_span));
    };

    ASSERT_TRUE(test(1));
    ASSERT_TRUE(test(2));
    ASSERT_TRUE(test(3));
}


TEST(MarkdownParserTest, ParseHeader) {

    {
        std::wstring expected_string = L"line1\nHeader2\nline2\nHeader3";
        StyleSpan root_style_span(0, expected_string.length(), StyleType::Normal, {
            { 6, 7, StyleType::Header2 },
            { 20, 7, StyleType::Header3 },
        });
        ASSERT_TRUE(TestParse(
            L"line1 \n\n## Header2  \n line2 \n\n###   Header3",
            StyledString(expected_string, root_style_span)));
    }
}


TEST(MarkdownParserTest, ParseInlineCode) {

    {
        std::wstring expected_string = L"This is an inline code.";
        StyleSpan root_style_span(0, expected_string.length(), StyleType::Normal, {
            { 0, expected_string.length(), StyleType::InlineCode }
        });
        ASSERT_TRUE(TestParse(
            L"`This is an inline code.`", 
            StyledString(expected_string, root_style_span)));
    }

    {
        std::wstring expected_string = L"This is an inline code.";
        StyleSpan root_style_span(0, expected_string.length(), StyleType::Normal, {
            { 11, 6, StyleType::InlineCode }
        });
        ASSERT_TRUE(TestParse(
            L"This is an `inline` code.",
            StyledString(expected_string, root_style_span)));
    }
}