#include <gtest/gtest.h>
#include "module/tool/date/date_command_factory.h"

using namespace ra::module::tool::date;
using namespace ra::utility;

/* 
Move to unit test of ToolModule.
TEST(DateCommandFactoryTest, ParseNotDateCommand) {

	auto result = DateCommandFactory::Parse(CommandLine{ L"" });
	ASSERT_FALSE(result.has_value());

	result = DateCommandFactory::Parse(CommandLine{ L"datetime" });
	ASSERT_FALSE(result.has_value());

	result = DateCommandFactory::Parse(CommandLine{ L"dat" });
	ASSERT_FALSE(result.has_value());

	result = DateCommandFactory::Parse(CommandLine{ L"data" });
	ASSERT_FALSE(result.has_value());

	result = DateCommandFactory::Parse(CommandLine{ L"Date" });
	ASSERT_FALSE(result.has_value());

	result = DateCommandFactory::Parse(CommandLine{ L"DATE" });
	ASSERT_FALSE(result.has_value());
}
*/


TEST(DateCommandFactoryTest, ParseCurrentDateTime) {

	auto result = DateCommandFactory::Parse(CommandLine{ L"date" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = DateCommandFactory::Parse(CommandLine{ L"date /e" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandFactoryTest, ParseTimeStamp) {

	auto result = DateCommandFactory::Parse(CommandLine{ L"date 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_FALSE(result->output_raw_value);

	result = DateCommandFactory::Parse(CommandLine{ L"date /e 10000" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 10000);
	ASSERT_TRUE(result->output_raw_value);
}


TEST(DateCommandFactoryTest, ParseIncompleteSwitch) {

	auto result = DateCommandFactory::Parse(CommandLine{ L"date /" });
	ASSERT_TRUE(result.has_value());
	ASSERT_FALSE(result->value.has_value());
	ASSERT_FALSE(result->output_raw_value);

	result = DateCommandFactory::Parse(CommandLine{ L"date / 100" });
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(*result->value, 100);
	ASSERT_FALSE(result->output_raw_value);
}