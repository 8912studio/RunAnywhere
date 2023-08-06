#include <gtest/gtest.h>
#include "module/tool/date/date_command_parsing.h"

using namespace ra::mod::tool::date;
using namespace ra::utility;

TEST(DateCommandParingTest, ParseCurrentDateTime) {

	auto result = Parse(CommandLine{ L"date" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = Parse(CommandLine{ L"date /e" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandParingTest, ParseTimeStamp) {

	auto result = Parse(CommandLine{ L"date 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_FALSE(result->output_raw_value);

	result = Parse(CommandLine{ L"date /e 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandParingTest, ParseIncompleteSwitch) {

	auto result = Parse(CommandLine{ L"date /" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = Parse(CommandLine{ L"date / 100" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 100);
	ASSERT_FALSE(result->output_raw_value);
}