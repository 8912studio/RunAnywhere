#include <gtest/gtest.h>
#include <zaf/base/range.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/paragraph_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

TEST(ParagraphParserTest, Failure) {

    ParseContext context(L"");
    auto element = ParagraphParser::Instance()->Parse(context);
    ASSERT_EQ(element, nullptr);
}


TEST(ParagraphParserTest, Success) {

    auto test = [](std::wstring_view input, ElementList expected) {
        ParseContext context(input);
        auto element = ParagraphParser::Instance()->Parse(context);
        if (!element) {
            return false;
        }
        return element->IsEqualTo(*MakeParagraph(std::move(expected)));
    };

    ASSERT_TRUE(test(L"single line paragraph", { MakeText(L"single line paragraph") }));
    ASSERT_TRUE(test(L"line1\nline2", { MakeText(L"line1 line2") }));
    ASSERT_TRUE(test(L"line1\nline2\n", { MakeText(L"line1 line2") }));
    ASSERT_TRUE(test(L"line1 \nline2", { MakeText(L"line1 line2") }));
    ASSERT_TRUE(test(L"line1 \n line2", { MakeText(L"line1 line2") }));
    ASSERT_TRUE(test(L"line1  \nline2", { MakeText(L"line1\nline2") }));
    ASSERT_TRUE(test(L"line1   \n line2", { MakeText(L"line1\nline2") }));

    ASSERT_TRUE(test(L"``", { MakeText(L"``") }));
    ASSERT_TRUE(test(L"``text", { MakeText(L"``text") }));
    ASSERT_TRUE(test(L"``text`", { MakeText(L"``text`") }));
    ASSERT_TRUE(test(L"text`", { MakeText(L"text`") }));
    ASSERT_TRUE(test(L"`text``", { MakeText(L"`text``") }));
    ASSERT_TRUE(test(L"```", { MakeText(L"```") }));
    ASSERT_TRUE(test(L"`code`", { MakeInlineCode(L"code") }));
    ASSERT_TRUE(test(L" `code`  ", { MakeInlineCode(L"code") }));

    ASSERT_TRUE(test(L"``text`code`  ", { 
        MakeText(L"``text"),
        MakeInlineCode(L"code"),
    }));

    ASSERT_TRUE(test(L"`code`\ntext", { 
        MakeInlineCode(L"code"),
        MakeText(L" text"),
    }));

    ASSERT_TRUE(test(L"`code` \ntext", {
        MakeInlineCode(L"code"),
        MakeText(L" text"),
    }));

    ASSERT_TRUE(test(L"`code`  \ntext", {
        MakeInlineCode(L"code"),
        MakeText(L"\ntext"),
    }));

    ASSERT_TRUE(test(L"text\n`code`", {
        MakeText(L"text "),
        MakeInlineCode(L"code"),
    }));

    ASSERT_TRUE(test(L"This is an `inline code`.", {
        MakeText(L"This is an "),
        MakeInlineCode(L"inline code"),
        MakeText(L"."),
    }));

    ASSERT_TRUE(test(L"**`code**`", {
        MakeText(L"**"),
        MakeInlineCode(L"code**"),
    }));

    ASSERT_TRUE(test(L"***abc***", {
        MakeItalics({ 
            MakeBold({ MakeText(L"abc") })
        })
    }));

    ASSERT_TRUE(test(L"***abc**", {
        MakeText(L"*"),
        MakeBold({ MakeText(L"abc") }),
    }));

    ASSERT_TRUE(test(L"**abc***", {
        MakeBold({ MakeText(L"abc") }),
        MakeText(L"*"),
    }));
}
