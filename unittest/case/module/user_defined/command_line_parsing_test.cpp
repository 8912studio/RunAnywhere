#include <gtest/gtest.h>
#include "module/user_defined/command_line_parsing.h"

using namespace ra::context;
using namespace ra::module::user_defined;

TEST(CommandLineParsingTest, ReplaceActivePath) {

    //Empty path
    auto result = ParseCommandLine(L"C:\\Window\\notepad.exe %@", ActivePath{}, {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 0);

    //No space in path
    result = ParseCommandLine(
        L"C:\\Window\\notepad.exe %@", 
        ActivePath{ L"C:\\Documents\\file.txt" },
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Documents\\file.txt");

    //There is space in path
    result = ParseCommandLine(
        L"C:\\Window\\notepad.exe %@",
        ActivePath{ L"C:\\My Documents\\file.txt" },
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"\"C:\\My Documents\\file.txt\"");

    //Parameter is quoted
    result = ParseCommandLine(
        L"C:\\Window\\notepad.exe \"%@\"", 
        ActivePath{ L"C:\\My Documents\\file.txt" },
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"\"C:\\My Documents\\file.txt\"");

    //Path as a part of parameter
    result = ParseCommandLine(
        L"C:\\Window\\notepad.exe /path:%@", 
        ActivePath{ L"C:\\file.txt" },
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"/path:C:\\file.txt");
}


TEST(CommandLineParsingTest, BackwardActivePath) {

    auto result = ParseCommandLine(
        L"C:\\Windows\\notepad.exe %.@",
        ActivePath{ L"C:\\Windows\\System32" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Windows\\System32");

    result = ParseCommandLine(
        L"C:\\Windows\\notepad.exe %.@",
        ActivePath{ L"C:\\Windows\\System32\\ntdll.dll" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Windows\\System32");

    result = ParseCommandLine(
        L"C:\\Windows\\notepad.exe %..@",
        ActivePath{ L"C:\\Windows\\System32\\ntdll.dll" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Windows");

    result = ParseCommandLine(
        L"C:\\Windows\\notepad.exe %...@",
        ActivePath{ L"C:\\Windows\\System32\\ntdll.dll" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\");

    result = ParseCommandLine(
        L"C:\\Windows\\notepad.exe %....@",
        ActivePath{ L"C:\\Windows\\System32\\ntdll.dll" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\");
}