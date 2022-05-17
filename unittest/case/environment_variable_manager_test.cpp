#include <gtest/gtest.h>
#include "environment_variable_manager.h"

using namespace ra;

TEST(EnvironmentVariableManagerTest, ParseEnvironmentStrings) {

    {
        const wchar_t* strings = L"\0\0";
        auto result = EnvironmentVariableManager::ParseEnvironmentStrings(strings);
        ASSERT_TRUE(result.empty());
    }

    {
        const wchar_t* strings = L"DebugPath=C:\\zplutor\0\0";
        auto result = EnvironmentVariableManager::ParseEnvironmentStrings(strings);
        ASSERT_EQ(result.size(), 1);
        ASSERT_EQ(result[L"DebugPath"], L"C:\\zplutor");
    }

    {
        const wchar_t* strings = L"First=111\0Second=2222\0Third=3\0\0";
        auto result = EnvironmentVariableManager::ParseEnvironmentStrings(strings);
        ASSERT_EQ(result.size(), 3);
        ASSERT_EQ(result[L"First"], L"111");
        ASSERT_EQ(result[L"Second"], L"2222");
        ASSERT_EQ(result[L"Third"], L"3");
    }

    {
        const wchar_t* strings = L"Arguments=--path=cc\0PPP=None\0\0";
        auto result = EnvironmentVariableManager::ParseEnvironmentStrings(strings);
        ASSERT_EQ(result.size(), 2);
        ASSERT_EQ(result[L"Arguments"], L"--path=cc");
        ASSERT_EQ(result[L"PPP"], L"None");
    }
}
