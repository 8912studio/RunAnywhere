#include <gtest/gtest.h>
#include "utility/numeric_text_formatting.h"

using namespace ra::utility;

TEST(NumericTextFormattingTest, Decimal) {

	auto test = [](const std::wstring& input, const std::wstring& expected) {
	
		std::wstring result = input;
		InsertSeperatorToNumericText(result, 10, L' ');
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
		InsertSeperatorToNumericText(result, 2, L' ');
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


TEST(NumericTextFormattingTest, Hex) {
	
	auto test = [](const std::wstring& input, const std::wstring& expected) {

		auto result = input;
		InsertSeperatorToNumericText(result, 16, L' ');
		return result == expected;
	};

	ASSERT_TRUE(test(L"", L""));
	ASSERT_TRUE(test(L"a", L"a"));
	ASSERT_TRUE(test(L"ab", L"ab"));
	ASSERT_TRUE(test(L"abc", L"abc"));
	ASSERT_TRUE(test(L"abcd", L"abcd"));
	ASSERT_TRUE(test(L"abcde", L"abcde"));
	ASSERT_TRUE(test(L"abcdef", L"abcdef"));
	ASSERT_TRUE(test(L"abcdef1", L"abcdef1"));
	ASSERT_TRUE(test(L"abcdef12", L"abcdef12"));
	ASSERT_TRUE(test(L"abcdef123", L"a bcdef123"));
	ASSERT_TRUE(test(L"abcdef1234", L"ab cdef1234"));
	ASSERT_TRUE(test(L"abcdef12345678", L"abcdef 12345678"));
}


TEST(NumericTextFormattingTest, DifferentSeperator) {

	std::wstring text(L"9876543210");
	InsertSeperatorToNumericText(text, 10, L',');
	ASSERT_EQ(text, L"9,876,543,210");

	text = L"9876543210";
	InsertSeperatorToNumericText(text, 10, L'-');
	ASSERT_EQ(text, L"9-876-543-210");
}