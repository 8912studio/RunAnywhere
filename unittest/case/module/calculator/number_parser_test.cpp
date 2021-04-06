#include <gtest/gtest.h>
#include "number_parser_test_utility.h"
#include "module/calculator/parse/number_parser.h"

TEST(NumberParseTest, Parse) {

    auto test = [](
        const std::wstring& input, 
        const std::wstring& expected_output, 
        int expected_base) {

        return TestNumberParserSuccess(
            *calculator::NumberParser::Instance(),
            input, 
            expected_output, 
            expected_base);
    };

    ASSERT_TRUE(test(L"0", L"0", 10));
    ASSERT_TRUE(test(L"0.78", L"0.78", 10));

    ASSERT_TRUE(test(L"00", L"0", 8));
    ASSERT_TRUE(test(L"046", L"46", 8));

    ASSERT_TRUE(test(L"0b110", L"110", 2));
    ASSERT_TRUE(test(L"0B110", L"110", 2));
    ASSERT_TRUE(test(L"b110001", L"110001", 2));
    ASSERT_TRUE(test(L"B110001", L"110001", 2));

    ASSERT_TRUE(test(L"0x91f3ef", L"91f3ef", 16));
    ASSERT_TRUE(test(L"0X91f3ef", L"91f3ef", 16));
    ASSERT_TRUE(test(L"x38ad3", L"38ad3", 16));
    ASSERT_TRUE(test(L"X38ad3", L"38ad3", 16));
    ASSERT_TRUE(test(L"0x4Ad3bF", L"4Ad3bF", 16));
}