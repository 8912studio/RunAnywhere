#include <gtest/gtest.h>
#include "utility/text_utility.h"

using namespace ra::utility;

TEST(TextUtilityTest, DeterminateLineBreakInfo) {

    auto test = [](std::wstring_view text, LineBreak first, bool all_the_same) {
        auto result = DeterminateLineBreakInfo(text);
        return 
            result.first_line_break == first && 
            result.all_the_same == all_the_same;
    };

    ASSERT_TRUE(test(L"", LineBreak::CRLF, true));
    ASSERT_TRUE(test(L"abcde", LineBreak::CRLF, true));

    ASSERT_TRUE(test(L"11\r\n", LineBreak::CRLF, true));
    ASSERT_TRUE(test(L"11\r\n3232", LineBreak::CRLF, true));
    ASSERT_TRUE(test(L"11\r\n", LineBreak::CRLF, true));
    ASSERT_TRUE(test(L"11\r\n3232\r\n\r\n", LineBreak::CRLF, true));
    ASSERT_TRUE(test(L"\r\n11\r\n3232\r\n", LineBreak::CRLF, true));
    ASSERT_TRUE(test(L"11\r\n3232\r\r\n", LineBreak::CRLF, false));

    ASSERT_TRUE(test(L"\n11\n", LineBreak::LF, true));
    ASSERT_TRUE(test(L"11\n", LineBreak::LF, true));
    ASSERT_TRUE(test(L"11\n\n", LineBreak::LF, true));
    ASSERT_TRUE(test(L"11\n22\n", LineBreak::LF, true));
    ASSERT_TRUE(test(L"11\n22\r33", LineBreak::LF, false));
    ASSERT_TRUE(test(L"11\n22\r\n33", LineBreak::LF, false));
    ASSERT_TRUE(test(L"11\n\r", LineBreak::LF, false));

    ASSERT_TRUE(test(L"\r11\r", LineBreak::CR, true));
    ASSERT_TRUE(test(L"11\r\r", LineBreak::CR, true));
    ASSERT_TRUE(test(L"11\r22\n", LineBreak::CR, false));
    ASSERT_TRUE(test(L"11\r22\r\n", LineBreak::CR, false));
}


TEST(TextUtilityTest, RemoveMultipleLines) {

    auto test = [](std::wstring input, const std::wstring& expected) {
        RemoveMultipleLines(input);
        return input == expected;
    };

    ASSERT_TRUE(test(L"bacde", L"bacde"));
    ASSERT_TRUE(test(L"first\rsecond", L"first"));
    ASSERT_TRUE(test(L"first\nsecond", L"first"));
    ASSERT_TRUE(test(L"first\r\nsecond", L"first"));
    ASSERT_TRUE(test(L"first\r\nsecond\r\nthird", L"first"));
}