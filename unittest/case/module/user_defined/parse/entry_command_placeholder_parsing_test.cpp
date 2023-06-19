#include <gtest/gtest.h>
#include "module/user_defined/parse/entry_command_placeholder_parsing.h"

using namespace ra::mod::user_defined;

TEST(EntryCommandPlaceholderParsingTest, ParseNoPlaceholder) {

	auto result = ParseEntryCommandPlaceholders(L"");
	ASSERT_EQ(result.size(), 0);

	result = ParseEntryCommandPlaceholders(L"192 3 472 d231 ddf");
	ASSERT_EQ(result.size(), 0);
}


TEST(EntryCommandPlaceholderParsingTest, ParseGeneral) {

	auto test = [](std::wstring_view string, std::size_t expected_index) {

		auto result = ParseEntryCommandPlaceholders(string);
		if (result.size() != 1) {
			return false;
		}

		const auto& parameter = result.front();
		return
			parameter.position == 0 &&
			parameter.length == string.size() &&
			parameter.index == expected_index;
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


TEST(EntryCommandPlaceholderParsingTest, ParseQuoted) {

	auto result = ParseEntryCommandPlaceholders(L"%2\"%1\"");
	ASSERT_EQ(result.size(), 2);

	const auto& second = result[0];
	ASSERT_EQ(second.position, 0);
	ASSERT_EQ(second.length, 2);
	ASSERT_EQ(second.index, 2);

	const auto& third = result[1];
	ASSERT_EQ(third.position, 3);
	ASSERT_EQ(third.length, 2);
	ASSERT_EQ(third.index, 1);
}


TEST(EntryCommandPlaceholderParsingTest, ParseCombine) {

	auto result = ParseEntryCommandPlaceholders(L"/path:%1;;%3\"%4\"--");
	ASSERT_EQ(result.size(), 3);

	const auto& second = result[0];
	ASSERT_EQ(second.position, 6);
	ASSERT_EQ(second.length, 2);
	ASSERT_EQ(second.index, 1);

	const auto& third = result[1];
	ASSERT_EQ(third.position, 10);
	ASSERT_EQ(third.length, 2);
	ASSERT_EQ(third.index, 3);

	const auto& fourth = result[2];
	ASSERT_EQ(fourth.position, 13);
	ASSERT_EQ(fourth.length, 2);
	ASSERT_EQ(fourth.index, 4);
}


TEST(EntryCommandPlaceholderParsingTest, InvalidPlaceholders) {

	auto result = ParseEntryCommandPlaceholders(L"%d %..2");
	ASSERT_TRUE(result.empty());

	result = ParseEntryCommandPlaceholders(L"%a %3 %~!1 %1");
	ASSERT_EQ(result.size(), 2);

	const auto& first = result[0];
	ASSERT_EQ(first.position, 3);
	ASSERT_EQ(first.length, 2);
	ASSERT_EQ(first.index, 3);

	const auto& second = result[1];
	ASSERT_EQ(second.position, 11);
	ASSERT_EQ(second.length, 2);
	ASSERT_EQ(second.index, 1);
}