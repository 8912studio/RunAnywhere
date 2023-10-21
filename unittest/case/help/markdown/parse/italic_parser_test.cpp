#include <gtest/gtest.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/italic_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

TEST(ItalicsParserTest, Success) {

    auto test = [](std::wstring_view input, ElementList expected) {
        ParseContext context(input);
        auto element = ItalicParser::Instance()->Parse(context);
        if (!element) {
            return false;
        }
        return element->IsEqualTo(*MakeItalic(std::move(expected)));
    };

    ASSERT_TRUE(test(L"*abc*", { MakeText(L"abc") }));
    ASSERT_TRUE(test(L"***abc***", { MakeBold({ MakeText(L"abc") }) }));
    ASSERT_TRUE(test(L"*`abc`*", { MakeInlineCode(L"abc") }));
    ASSERT_TRUE(test(L"***`abc`***", {
        MakeBold({ MakeInlineCode(L"abc") }),
    }));
    ASSERT_TRUE(test(L"*head**`abc`tail***", {
        MakeText(L"head"),
        MakeBold({ 
            MakeInlineCode(L"abc"), 
            MakeText(L"tail")
        }),
    }));
}


TEST(ItalicsParserTest, Failure) {

    auto test = [](std::wstring_view input) {
        ParseContext context(input);
        auto element = ItalicParser::Instance()->Parse(context);
        return !element && (context.CurrentIndex() == 0);
    };

    ASSERT_TRUE(test(L"**abcd**"));
    ASSERT_TRUE(test(L"*abcd"));
    ASSERT_TRUE(test(L"abcd*"));
    ASSERT_TRUE(test(L"* abcd *"));
    ASSERT_TRUE(test(L"*abcd *"));
    ASSERT_TRUE(test(L"* abcd*"));
    ASSERT_TRUE(test(L"**"));
    ASSERT_TRUE(test(L"***"));
}