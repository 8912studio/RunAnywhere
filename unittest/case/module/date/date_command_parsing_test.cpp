#include <gtest/gtest.h>
#include "module/date/date_command_parsing.h"

using namespace ra::module::date;
using namespace ra::utility;

TEST(DateCommandParsingTest, NotDateCommand) {

	auto result = ParseDateCommand(CommandLine{ L"" });
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(CommandLine{ L"datetime" });
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(CommandLine{ L"dat" });
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(CommandLine{ L"data" });
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(CommandLine{ L"Date" });
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(CommandLine{ L"DATE" });
	ASSERT_FALSE(result.has_value());
}


TEST(DateCommandParsingTest, CurrentDateTime) {

	auto result = ParseDateCommand(CommandLine{ L"date" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = ParseDateCommand(CommandLine{ L"date /e" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandParsingTest, TimeStamp) {

	auto result = ParseDateCommand(CommandLine{ L"date 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_FALSE(result->output_raw_value);

	result = ParseDateCommand(CommandLine{ L"date /e 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandParsingTest, IncompleteSwitch) {

	auto result = ParseDateCommand(CommandLine{ L"date /" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = ParseDateCommand(CommandLine{ L"date / 100" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 100);
	ASSERT_FALSE(result->output_raw_value);
}