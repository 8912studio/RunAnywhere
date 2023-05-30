#include <gtest/gtest.h>
#include "utility/text_utility.h"

using namespace ra::utility;

TEST(DeterminateLineBreakTest, Determinate) {

    ASSERT_EQ(DeterminateLineBreak(L""), LineBreak::CRLF);
    ASSERT_EQ(DeterminateLineBreak(L"abcde"), LineBreak::CRLF);
    ASSERT_EQ(DeterminateLineBreak(L"11\r"), LineBreak::CR);
    ASSERT_EQ(DeterminateLineBreak(L"11\r\r"), LineBreak::CR);
    ASSERT_EQ(DeterminateLineBreak(L"11\n"), LineBreak::LF);
    ASSERT_EQ(DeterminateLineBreak(L"11\n\r"), LineBreak::LF);
    ASSERT_EQ(DeterminateLineBreak(L"11\r\n"), LineBreak::CRLF);
    ASSERT_EQ(DeterminateLineBreak(L"11\r\n3232"), LineBreak::CRLF);
}