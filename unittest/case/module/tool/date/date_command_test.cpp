#include <gtest/gtest.h>
#include "module/tool/date/date_command.h"

using namespace ra::module::tool::date;
using namespace ra::utility;

TEST(DateCommandTest, ParseCurrentDateTime) {

	auto result = DateCommand::Parse(CommandLine{ L"date" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = DateCommand::Parse(CommandLine{ L"date /e" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandTest, ParseTimeStamp) {

	auto result = DateCommand::Parse(CommandLine{ L"date 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_FALSE(result->output_raw_value);

	result = DateCommand::Parse(CommandLine{ L"date /e 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandTest, ParseIncompleteSwitch) {

	auto result = DateCommand::Parse(CommandLine{ L"date /" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = DateCommand::Parse(CommandLine{ L"date / 100" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 100);
	ASSERT_FALSE(result->output_raw_value);
}