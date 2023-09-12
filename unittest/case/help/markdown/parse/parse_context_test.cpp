#include <gtest/gtest.h>
#include "help/markdown/parse/parse_context.h"

using namespace ra::help::markdown::parse;

TEST(ParseContextTest, LineBreak) {

    auto test = [](std::wstring_view input) {
    
        ParseContext context(input);
        bool expected =
            context.IsAtLineStart() &&
            !context.IsAtLineEnd() &&
            context.CurrentChar() == L'a' &&
            context.PreviousChar() == std::nullopt;

        if (!expected) {
            return false;
        }

        context.Forward();
        expected =
            !context.IsAtLineStart() &&
            context.IsAtLineEnd() &&
            context.CurrentChar() == L'\n' &&
            context.PreviousChar() == L'a';

        if (!expected) {
            return false;
        }

        context.Forward();
        expected =
            context.IsAtLineStart() &&
            !context.IsAtLineEnd() &&
            context.CurrentChar() == L'b' &&
            context.PreviousChar() == L'\n';

        if (!expected) {
            return false;
        }

        context.Forward();
        expected =
            !context.IsAtLineStart() &&
            context.IsAtLineEnd() &&
            context.CurrentChar() == 0 &&
            context.PreviousChar() == L'b';

        return expected;
    };

    ASSERT_TRUE(test(L"a\rb"));
    ASSERT_TRUE(test(L"a\nb"));
    ASSERT_TRUE(test(L"a\r\nb"));
}