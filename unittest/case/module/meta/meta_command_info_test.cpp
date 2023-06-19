#include <gtest/gtest.h>
#include "module/meta/meta_command_info.h"

using namespace ra::mod::meta;

TEST(MetaCommandInfoTest, GetMetaCommandInfoWithLowercase) {

	ASSERT_NE(GetMetaCommandInfo(L"option"), nullptr);
	ASSERT_NE(GetMetaCommandInfo(L"about"), nullptr);
	ASSERT_NE(GetMetaCommandInfo(L"reload"), nullptr);
}


TEST(MetaCommandInfoTest, GetMetaCommandInfoWithUppercase) {

	ASSERT_NE(GetMetaCommandInfo(L"OPTION"), nullptr);
	ASSERT_NE(GetMetaCommandInfo(L"ABOUT"), nullptr);
	ASSERT_NE(GetMetaCommandInfo(L"RELOAD"), nullptr);
}