#include <gtest/gtest.h>
#include "module/user_defined/command_parameter_parsing.h"

TEST(CommandParameterParsingTest, ParseNoParameter) {

	auto result = ParseParameter(L"");
	ASSERT_EQ(result.size(), 0);

	result = ParseParameter(L"192 3 472 d231 ddf");
	ASSERT_EQ(result.size(), 0);
}


TEST(CommandParameterParsingTest, ParseActivePath) {

	auto test = [](std::wstring_view string) {
	
		auto result = ParseParameter(string);
		if (result.size() != 1) {
			return false;
		}

		const auto& parameter = result.front();
		return
			parameter.type == CommandParameter::Type::ActivePath &&
			parameter.position == 0 &&
			parameter.length == string.size() &&
			parameter.backward_level == string.size() - 2 &&
			parameter.general_index == 0 &&
			parameter.is_quoted == false;
	};

	ASSERT_TRUE(test(L"%@"));
	ASSERT_TRUE(test(L"%.@"));
	ASSERT_TRUE(test(L"%..@"));
	ASSERT_TRUE(test(L"%...@"));
	ASSERT_TRUE(test(L"%....@"));
	ASSERT_TRUE(test(L"%.....@"));
}


TEST(CommandParameterParsingTest, ParseGeneral) {

	auto test = [](std::wstring_view string, std::size_t expected_index) {

		auto result = ParseParameter(string);
		if (result.size() != 1) {
			return false;
		}

		const auto& parameter = result.front();
		return
			parameter.type == CommandParameter::Type::General &&
			parameter.position == 0 &&
			parameter.length == string.size() &&
			parameter.general_index == expected_index &&
			parameter.backward_level == 0 &&
			parameter.is_quoted == false;
	};

	ASSERT_TRUE(test(L"%1", 1));
	ASSERT_TRUE(test(L"%2", 2));
	ASSERT_TRUE(test(L"%3", 3));
	ASSERT_TRUE(test(L"%4", 4));
	ASSERT_TRUE(test(L"%5", 5));
	ASSERT_TRUE(test(L"%6", 6));
	ASSERT_TRUE(test(L"%7", 7));
	ASSERT_TRUE(test(L"%8", 8));
	ASSERT_TRUE(test(L"%9", 9));
}


TEST(CommandParameterParsingTest, ParseQuoted) {

	auto result = ParseParameter(L"\"%...@\"%2\"%1\"");
	ASSERT_EQ(result.size(), 3);

	const auto& first = result[0];
	ASSERT_EQ(first.type, CommandParameter::Type::ActivePath);
	ASSERT_EQ(first.position, 1);
	ASSERT_EQ(first.length, 5);
	ASSERT_EQ(first.general_index, 0);
	ASSERT_EQ(first.backward_level, 3);
	ASSERT_EQ(first.is_quoted, true);

	const auto& second = result[1];
	ASSERT_EQ(second.type, CommandParameter::Type::General);
	ASSERT_EQ(second.position, 7);
	ASSERT_EQ(second.length, 2);
	ASSERT_EQ(second.general_index, 2);
	ASSERT_EQ(second.backward_level, 0);
	ASSERT_EQ(second.is_quoted, false);

	const auto& third = result[2];
	ASSERT_EQ(third.type, CommandParameter::Type::General);
	ASSERT_EQ(third.position, 10);
	ASSERT_EQ(third.length, 2);
	ASSERT_EQ(third.general_index, 1);
	ASSERT_EQ(third.backward_level, 0);
	ASSERT_EQ(third.is_quoted, true);
}


TEST(CommandParameterParsingTest, ParseCombine) {

	auto result = ParseParameter(L"/path:%..@,%1;;%3%4--\"%@\"");
	ASSERT_EQ(result.size(), 5);

	const auto& first = result[0];
	ASSERT_EQ(first.type, CommandParameter::Type::ActivePath);
	ASSERT_EQ(first.position, 6);
	ASSERT_EQ(first.length, 4);
	ASSERT_EQ(first.general_index, 0);
	ASSERT_EQ(first.backward_level, 2);
	ASSERT_EQ(first.is_quoted, false);

	const auto& second = result[1];
	ASSERT_EQ(second.type, CommandParameter::Type::General);
	ASSERT_EQ(second.position, 11);
	ASSERT_EQ(second.length, 2);
	ASSERT_EQ(second.general_index, 1);
	ASSERT_EQ(second.backward_level, 0);
	ASSERT_EQ(first.is_quoted, false);

	const auto& third = result[2];
	ASSERT_EQ(third.type, CommandParameter::Type::General);
	ASSERT_EQ(third.position, 15);
	ASSERT_EQ(third.length, 2);
	ASSERT_EQ(third.general_index, 3);
	ASSERT_EQ(third.backward_level, 0);
	ASSERT_EQ(first.is_quoted, false);

	const auto& fourth = result[3];
	ASSERT_EQ(fourth.type, CommandParameter::Type::General);
	ASSERT_EQ(fourth.position, 17);
	ASSERT_EQ(fourth.length, 2);
	ASSERT_EQ(fourth.general_index, 4);
	ASSERT_EQ(fourth.backward_level, 0);
	ASSERT_EQ(first.is_quoted, false);

	const auto& fifth = result[4];
	ASSERT_EQ(fifth.type, CommandParameter::Type::ActivePath);
	ASSERT_EQ(fifth.position, 22);
	ASSERT_EQ(fifth.length, 2);
	ASSERT_EQ(fifth.general_index, 0);
	ASSERT_EQ(fifth.backward_level, 0);
	ASSERT_EQ(fifth.is_quoted, true);
}


TEST(CommandParameterParsingTest, ParseUnknown) {

	auto result = ParseParameter(L"%d %..2");
	ASSERT_EQ(result.size(), 2);

	const auto& first = result[0];
	ASSERT_EQ(first.type, CommandParameter::Type::Unknown);
	ASSERT_EQ(first.position, 0);
	ASSERT_EQ(first.length, 1);
	ASSERT_EQ(first.general_index, 0);
	ASSERT_EQ(first.backward_level, 0);

	const auto& second = result[1];
	ASSERT_EQ(second.type, CommandParameter::Type::Unknown);
	ASSERT_EQ(second.position, 3);
	ASSERT_EQ(second.length, 1);
	ASSERT_EQ(second.general_index, 0);
	ASSERT_EQ(second.backward_level, 0);
}