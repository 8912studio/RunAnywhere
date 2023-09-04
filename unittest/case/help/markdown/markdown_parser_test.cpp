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
}