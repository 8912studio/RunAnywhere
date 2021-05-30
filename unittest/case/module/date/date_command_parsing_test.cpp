#include <gtest/gtest.h>
#include "module/date/date_command_parsing.h"

using namespace ra::module::date;

TEST(DateCommandParsingTest, NotDateCommand) {

	auto result = ParseDateCommand(L"");
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(L"datetime");
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(L"dat");
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(L"data");
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(L"Date");
	ASSERT_FALSE(result.has_value());

	result = ParseDateCommand(L"DATE");
	ASSERT_FALSE(result.has_value());
}


TEST(DateCommandParsingTest, CurrentDateTime) {

	auto result = ParseDateCommand(L"date");
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = ParseDateCommand(L"date /e");
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandParsingTest, TimeStamp) {

	auto result = ParseDateCommand(L"date 10000");
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_FALSE(result->output_raw_value);

	result = ParseDateCommand(L"date /e 10000");
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandParsingTest, IncompleteSwitch) {

	auto result = ParseDateCommand(L"date /");
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = ParseDateCommand(L"date / 100");
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 100);
	ASSERT_FALSE(result->output_raw_value);
}