#include <gtest/gtest.h>
#include "module/calculator/preview/numeric_text_formatting.h"

using namespace ra::module::calculator;

TEST(NumericTextFormattingTest, Decimal) {

	auto test = [](const std::wstring& input, const std::wstring& expected) {
	
		std::wstring result = input;
		InsertSpaceToNumericText(result, 10);
		return result == expected;
	};

	ASSERT_TRUE(test(L"", L""));
	ASSERT_TRUE(test(L"2", L"2"));
	ASSERT_TRUE(test(L"34", L"34"));
	ASSERT_TRUE(test(L"100", L"100"));
	ASSERT_TRUE(test(L"1000", L"1 000"));
	ASSERT_TRUE(test(L"1234567", L"1 234 567"));
	ASSERT_TRUE(test(L"123456789", L"123 456 789"));
	ASSERT_TRUE(test(L"0.8378218238238", L"0.8378218238238"));
	ASSERT_TRUE(test(L"123456789.8378218238238", L"123 456 789.8378218238238"));
}


TEST(NumericTextFormattingTest, Binary) {

	auto test = [](const std::wstring& input, const std::wstring& expected) {

		std::wstring result = input;
		InsertSpaceToNumericText(result, 2);
		return result == expected;
	};

	ASSERT_TRUE(test(L"", L""));
	ASSERT_TRUE(test(L"1", L"1"));
	ASSERT_TRUE(test(L"111", L"111"));
	ASSERT_TRUE(test(L"1011", L"1011"));
	ASSERT_TRUE(test(L"10001", L"10001"));
	ASSERT_TRUE(test(L"1111111", L"1111111"));
	ASSERT_TRUE(test(L"11111111", L"11111111"));
	ASSERT_TRUE(test(L"111111111", L"1 11111111"));
	ASSERT_TRUE(test(L"1111111100000000", L"11111111 00000000"));
}