#include <gtest/gtest.h>
#include "module/tool/json/json_command_preview_control.h"

using namespace ra::mod::tool::json;

TEST(JSONCommandPreviewControlTest, GetAdjacentLinesAtErrorIndex_SingleLine) {

    auto test = [](std::size_t error_index, std::size_t expected_char_index) {
        auto result = JSONCommandPreviewControl::GetAdjacentLinesAtErrorIndex(L"abc", error_index);
        return 
            (result.lines == std::vector<std::wstring>{L"abc"}) &&
            (result.error_line_index == 0) &&
            (result.error_char_index == expected_char_index);
    };

    ASSERT_TRUE(test(0, 0));
    ASSERT_TRUE(test(1, 1));
    ASSERT_TRUE(test(2, 2));
    ASSERT_TRUE(test(3, 3));
    ASSERT_TRUE(test(4, 3));
}


TEST(JSONCommandPreviewControlTest, GetAdjacentLinesAtErrorIndex_MultiLine) {

    auto test = [](
        std::size_t error_index, 
        std::size_t expected_line_index, 
        std::size_t expected_char_index) {

        auto result = JSONCommandPreviewControl::GetAdjacentLinesAtErrorIndex(
            L"1a\n2bb\r3ccc\r\n4dddd", 
            error_index);

        std::vector<std::wstring> expected_lines{ L"1a", L"2bb", L"3ccc", L"4dddd" };
        return
            (result.lines == expected_lines) &&
            (result.error_line_index == expected_line_index) &&
            (result.error_char_index == expected_char_index);
    };

    ASSERT_TRUE(test(0, 0, 0));
    ASSERT_TRUE(test(1, 0, 1));
    ASSERT_TRUE(test(2, 0, 2));
    ASSERT_TRUE(test(3, 1, 0));
    ASSERT_TRUE(test(4, 1, 1));
    ASSERT_TRUE(test(5, 1, 2));
    ASSERT_TRUE(test(6, 1, 3));
    ASSERT_TRUE(test(7, 2, 0));
    ASSERT_TRUE(test(8, 2, 1));
    ASSERT_TRUE(test(9, 2, 2));
    ASSERT_TRUE(test(10, 2, 3));
    ASSERT_TRUE(test(11, 2, 4));
    ASSERT_TRUE(test(12, 2, 4));
    ASSERT_TRUE(test(13, 3, 0));
    ASSERT_TRUE(test(14, 3, 1));
    ASSERT_TRUE(test(15, 3, 2));
    ASSERT_TRUE(test(16, 3, 3));
    ASSERT_TRUE(test(17, 3, 4));
    ASSERT_TRUE(test(18, 3, 5));
    ASSERT_TRUE(test(19, 3, 5));
}


TEST(JSONCommandPreviewControlTest, GetAdjacentLinesAtErrorIndex_AdjacentLines) {

    auto test = [](
        std::size_t error_index,
        std::size_t expected_line_index,
        const std::vector<std::wstring>& expected_lines) {

        auto result = JSONCommandPreviewControl::GetAdjacentLinesAtErrorIndex(
            L"000\n111\n222\n333\n444\n555\n666\n777\n888\n999",
            error_index);

        return 
            (result.lines == expected_lines) &&
            (result.error_line_index == expected_line_index);
    };

    std::vector<std::wstring> expected{
        L"000", //<-
        L"111",
        L"222",
        L"333",
    };
    ASSERT_TRUE(test(0, 0, expected));

    expected = {
        L"000",
        L"111", //<-
        L"222",
        L"333",
        L"444",
    };
    ASSERT_TRUE(test(4, 1, expected));

    expected = {
        L"000",
        L"111",
        L"222", //<-
        L"333",
        L"444",
        L"555",
    };
    ASSERT_TRUE(test(8, 2, expected));

    expected = {
        L"000",
        L"111",
        L"222", 
        L"333", //<-
        L"444",
        L"555",
        L"666",
    };
    ASSERT_TRUE(test(12, 3, expected));

    expected = {
        L"111",
        L"222",
        L"333", 
        L"444", //<-
        L"555",
        L"666",
        L"777"
    };
    ASSERT_TRUE(test(16, 3, expected));

    expected = {
        L"222",
        L"333",
        L"444", 
        L"555", //<-
        L"666",
        L"777",
        L"888",
    };
    ASSERT_TRUE(test(20, 3, expected));

    expected = {
        L"333",
        L"444",
        L"555", 
        L"666", //<-
        L"777",
        L"888",
        L"999",
    };
    ASSERT_TRUE(test(24, 3, expected));

    expected = {
        L"444",
        L"555",
        L"666", 
        L"777", //<-
        L"888",
        L"999",
    };
    ASSERT_TRUE(test(28, 3, expected));

    expected = {
        L"555",
        L"666",
        L"777", 
        L"888", //<-
        L"999",
    };
    ASSERT_TRUE(test(32, 3, expected));

    expected = {
        L"666",
        L"777",
        L"888", 
        L"999", //<-
    };
    ASSERT_TRUE(test(36, 3, expected));
}