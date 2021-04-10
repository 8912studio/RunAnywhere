#include <gtest/gtest.h>
#include "module/user_defined/command_line_parsing.h"

TEST(CommandLineParsingTest, ReplaceFocusedPath) {

    //Empty focused path
    auto result = ParseCommandLine(L"C:\\Window\\notepad.exe %@", {}, {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 0);

    //No space in focused path
    result = ParseCommandLine(L"C:\\Window\\notepad.exe %@", L"C:\\Documents\\file.txt", {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Documents\\file.txt");

    //There is space in focused path
    result = ParseCommandLine(L"C:\\Window\\notepad.exe %@", L"C:\\My Documents\\file.txt", {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"\"C:\\My Documents\\file.txt\"");

    //Focused path as a part of parameter
    result = ParseCommandLine(L"C:\\Window\\notepad.exe /path:%@", L"C:\\file.txt", {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"/path:C:\\file.txt");
}