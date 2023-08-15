#include <gtest/gtest.h>
#include "module/tool/date/date_command_parsing.h"

using namespace ra::mod::tool::date;
using namespace ra::utility;

TEST(DateCommandParingTest, ParseCurrentDateTime) {

	auto result = Parse(CommandLine{ L"date" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_TRUE(result->adjustments.empty());
	ASSERT_FALSE(result->output_raw_value);

	result = Parse(CommandLine{ L"date /e" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_TRUE(result->adjustments.empty());
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandParingTest, ParseTimeStamp) {

	auto result = Parse(CommandLine{ L"date 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_TRUE(result->adjustments.empty());
	ASSERT_FALSE(result->output_raw_value);

	result = Parse(CommandLine{ L"date /e 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_TRUE(result->adjustments.empty());
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandParingTest, ParseIncompleteSwitch) {

	auto result = Parse(CommandLine{ L"date /" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_TRUE(result->adjustments.empty());
	ASSERT_FALSE(result->output_raw_value);

	result = Parse(CommandLine{ L"date / 100" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 100);
	ASSERT_TRUE(result->adjustments.empty());
	ASSERT_FALSE(result->output_raw_value);
}


TEST(DateCommandParsingTest, ParseAdjustment) {

	{
		auto result = Parse(CommandLine{ L"date +abc" });
		ASSERT_FALSE(result.has_value());
	}

	{
		auto result = Parse(CommandLine{ L"date -13s13" });
		ASSERT_FALSE(result.has_value());
	}

	{
		auto result = Parse(CommandLine{ L"date +" });
		ASSERT_TRUE(result.has_value());
		ASSERT_FALSE(result->value.has_value());
		ASSERT_FALSE(result->output_raw_value);
		ASSERT_TRUE(result->adjustments.empty());
	}

	{
		auto result = Parse(CommandLine{ L"date -" });
		ASSERT_TRUE(result.has_value());
		ASSERT_FALSE(result->value.has_value());
		ASSERT_FALSE(result->output_raw_value);
		ASSERT_TRUE(result->adjustments.empty());
	}

	{
		auto result = Parse(CommandLine{ L"date +1s -2min" });
		ASSERT_TRUE(result.has_value());
		ASSERT_FALSE(result->value.has_value());
		ASSERT_FALSE(result->output_raw_value);
		ASSERT_EQ(result->adjustments.size(), 2);
		ASSERT_EQ(result->adjustments[0].adjustment, 1);
		ASSERT_EQ(result->adjustments[0].unit, DateTimeUnit::Second);
		ASSERT_EQ(result->adjustments[1].adjustment, -2);
		ASSERT_EQ(result->adjustments[1].unit, DateTimeUnit::Minute);
	}
}


TEST(DateCommandParsingTest, ParseAdjustmentUnit) {

	auto test = [](std::wstring_view suffix, DateTimeUnit unit) {
	
		std::wstring command{ L"date +43" };
		command += suffix;
		auto result = Parse(CommandLine{ command });
		if (!result.has_value()) {
			return false;
		}
		if (result->adjustments.empty()) {
			return false;
		}
		return result->adjustments.front().unit == unit;
	};

	ASSERT_TRUE(test(L"", DateTimeUnit::Second));
	ASSERT_TRUE(test(L"s", DateTimeUnit::Second));
	ASSERT_TRUE(test(L"m", DateTimeUnit::Minute));
	ASSERT_TRUE(test(L"mi", DateTimeUnit::Minute));
	ASSERT_TRUE(test(L"min", DateTimeUnit::Minute));
	ASSERT_TRUE(test(L"h", DateTimeUnit::Hour));
	ASSERT_TRUE(test(L"d", DateTimeUnit::Day));
	ASSERT_TRUE(test(L"w", DateTimeUnit::Week));
	ASSERT_TRUE(test(L"mo", DateTimeUnit::Month));
	ASSERT_TRUE(test(L"mon", DateTimeUnit::Month));
	ASSERT_TRUE(test(L"y", DateTimeUnit::Year));
}


TEST(DateCommandParsingTest, ParseTextBlock) {

	{
		CommandLine command_line(L"date \ufffc \ufffc /e", [](int index) {
			std::wstring content;
			if (index == 0) {
				content = L"20000";
			}
			else if (index == 1) {
				content = L"+2d";
			}
			return CommandLinePiece(CommandLinePieceType::TextBlock, content);
		});
		auto result = Parse(command_line);
		ASSERT_TRUE(result.has_value());
		ASSERT_EQ(result->value, 20000);
		ASSERT_EQ(result->output_raw_value, true);
		ASSERT_EQ(result->adjustments.size(), 1);
		ASSERT_EQ(result->adjustments.front().adjustment, 2);
		ASSERT_EQ(result->adjustments.front().unit, DateTimeUnit::Day);
	}

	{
		CommandLine command_line(L"date \ufffc", [](int) {
			return CommandLinePiece(CommandLinePieceType::TextBlock, L"/e");
		});
		auto result = Parse(command_line);
		ASSERT_FALSE(result.has_value());
	}
}