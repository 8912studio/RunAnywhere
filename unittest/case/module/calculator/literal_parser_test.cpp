#include <gtest/gtest.h>
#include "module/calculator/parse/literal_parser.h"

using namespace ra::mod::calculator;

TEST(LiteralParserTest, Normal) {

	auto test = [](const std::wstring& string) {
	
		ParseContext context{ string };
		ParseResult result;

		LiteralParser parser{ string };
		auto status = parser.Parse(context, result);
		if (status != ParseStatus::Ok) {
			return false;
		}

		if (context.GetCurrentIndex() != context.GetLength()) {
			return false;
		}

		return context.GetLastParsedLength() == string.length();
	};

	ASSERT_TRUE(test(L"a"));
	ASSERT_TRUE(test(L"literal"));
}


TEST(LiteralParserTest, Mismatched) {

	auto test = [](const std::wstring& command) {

		ParseContext context{ command };
		ParseResult result;

		LiteralParser parser{ L"literal" };
		auto status = parser.Parse(context, result);
		if (status != ParseStatus::Mismatched) {
			return false;
		}

		if (context.GetCurrentIndex() != 0) {
			return false;
		}

		return context.GetLastParsedLength() == 0;
	};

	ASSERT_TRUE(test(L""));
	ASSERT_TRUE(test(L"eral"));
}


TEST(LiteralParserTest, Error) {

	ParseContext context{ L"Lit" };
	ParseResult result;

	LiteralParser parser{ L"Literal" };
	auto status = parser.Parse(context, result);
	ASSERT_EQ(status, ParseStatus::Error);
	ASSERT_EQ(context.GetCurrentIndex(), 3);
	ASSERT_EQ(context.GetLastParsedLength(), 3);
}