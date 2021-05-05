#include <gtest/gtest.h>
#include "module/active_path/active_path_option_parsing.h"

using namespace ra::module::active_path;

TEST(ActivePathOptionParsingTest, ParseInvalidTest) {

	auto option = ParseActivePathOption(L"");
	ASSERT_EQ(option.backward_level, 0);

	option = ParseActivePathOption(L"??");
	ASSERT_EQ(option.backward_level, 0);

	option = ParseActivePathOption(L"m");
	ASSERT_EQ(option.backward_level, 0);
}


TEST(ActivePathOptionParsingTest, ParseBackwardLevel) {

	auto option = ParseActivePathOption(L".");
	ASSERT_EQ(option.backward_level, 1);

	option = ParseActivePathOption(L"..");
	ASSERT_EQ(option.backward_level, 2);

	option = ParseActivePathOption(L"...");
	ASSERT_EQ(option.backward_level, 3);

	option = ParseActivePathOption(L"....");
	ASSERT_EQ(option.backward_level, 4);
}


TEST(ActivePathOptionParsingTest, ParseNameOnly) {

	auto option = ParseActivePathOption(L"");
	ASSERT_EQ(option.name_only, false);

	option = ParseActivePathOption(L"..");
	ASSERT_EQ(option.name_only, false);

	option = ParseActivePathOption(L"n");
	ASSERT_EQ(option.name_only, true);

	option = ParseActivePathOption(L".n");
	ASSERT_EQ(option.name_only, true);
}


TEST(ActivePathOptionParsingTest, Combine) {

	auto option = ParseActivePathOption(L"..n");
	ASSERT_EQ(option.backward_level, 2);
	ASSERT_EQ(option.name_only, true);
}