#include <gtest/gtest.h>
#include "module/user_defined/parse/entry_command_parameter_parsing.h"

using namespace ra::module::user_defined;

TEST(EntryCommandParameterParsingTest, ParseNoParameter) {

	auto result = ParseEntryCommandParameter(L"");
	ASSERT_EQ(result.size(), 0);

	result = ParseEntryCommandParameter(L"192 3 472 d231 ddf");
	ASSERT_EQ(result.size(), 0);
}


TEST(EntryCommandParameterParsingTest, ParseGeneral) {

	auto test = [](std::wstring_view string, std::size_t expected_index) {

		auto result = ParseEntryCommandParameter(string);
		if (result.size() != 1) {
			return false;
		}

		const auto& parameter = result.front();
		return
			parameter.type == EntryCommandParameterPart::Type::General &&
			parameter.position == 0 &&
			parameter.length == string.size() &&
			parameter.general_index == expected_index &&
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


TEST(EntryCommandParameterParsingTest, ParseQuoted) {

	auto result = ParseEntryCommandParameter(L"%2\"%1\"");
	ASSERT_EQ(result.size(), 2);

	const auto& second = result[0];
	ASSERT_EQ(second.type, EntryCommandParameterPart::Type::General);
	ASSERT_EQ(second.position, 0);
	ASSERT_EQ(second.length, 2);
	ASSERT_EQ(second.general_index, 2);
	ASSERT_EQ(second.is_quoted, false);

	const auto& third = result[1];
	ASSERT_EQ(third.type, EntryCommandParameterPart::Type::General);
	ASSERT_EQ(third.position, 3);
	ASSERT_EQ(third.length, 2);
	ASSERT_EQ(third.general_index, 1);
	ASSERT_EQ(third.is_quoted, true);
}


TEST(EntryCommandParameterParsingTest, ParseCombine) {

	auto result = ParseEntryCommandParameter(L"/path:%1;;%3\"%4\"--");
	ASSERT_EQ(result.size(), 3);

	const auto& second = result[0];
	ASSERT_EQ(second.type, EntryCommandParameterPart::Type::General);
	ASSERT_EQ(second.position, 6);
	ASSERT_EQ(second.length, 2);
	ASSERT_EQ(second.general_index, 1);
	ASSERT_EQ(second.is_quoted, false);

	const auto& third = result[1];
	ASSERT_EQ(third.type, EntryCommandParameterPart::Type::General);
	ASSERT_EQ(third.position, 10);
	ASSERT_EQ(third.length, 2);
	ASSERT_EQ(third.general_index, 3);
	ASSERT_EQ(third.is_quoted, false);

	const auto& fourth = result[2];
	ASSERT_EQ(fourth.type, EntryCommandParameterPart::Type::General);
	ASSERT_EQ(fourth.position, 13);
	ASSERT_EQ(fourth.length, 2);
	ASSERT_EQ(fourth.general_index, 4);
	ASSERT_EQ(fourth.is_quoted, true);
}


TEST(EntryCommandParameterParsingTest, ParseUnknown) {

	auto result = ParseEntryCommandParameter(L"%d %..2");
	ASSERT_EQ(result.size(), 2);

	const auto& first = result[0];
	ASSERT_EQ(first.type, EntryCommandParameterPart::Type::Unknown);
	ASSERT_EQ(first.position, 0);
	ASSERT_EQ(first.length, 1);
	ASSERT_EQ(first.general_index, 0);

	const auto& second = result[1];
	ASSERT_EQ(second.type, EntryCommandParameterPart::Type::Unknown);
	ASSERT_EQ(second.position, 3);
	ASSERT_EQ(second.length, 1);
	ASSERT_EQ(second.general_index, 0);
}