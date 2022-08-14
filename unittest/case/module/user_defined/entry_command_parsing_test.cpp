#include <gtest/gtest.h>
#include "module/user_defined/parse/entry_command_parsing.h"

using namespace ra::context;
using namespace ra::module::user_defined;

TEST(EntryCommandParsingTest, ReplaceActivePath) {

    //Empty path
    auto result = ParseEntryCommand(L"C:\\Window\\notepad.exe {@}", ActivePath{}, {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 0);

    //No space in path
    result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe {@}", 
        ActivePath{ L"C:\\Documents\\file.txt" },
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Documents\\file.txt");

    //There is space in path
    result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe {@}",
        ActivePath{ L"C:\\My Documents\\file.txt" },
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\My Documents\\file.txt");

    //Parameter is quoted
    result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe \"{@}\"", 
        ActivePath{ L"C:\\My Documents\\file.txt" },
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"\"C:\\My Documents\\file.txt\"");

    //Active path is a part of parameter
    result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe /path:{@}", 
        ActivePath{ L"C:\\file.txt" },
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"/path:C:\\file.txt");
}


TEST(EntryCommandParsingTest, BackwardActivePath) {

    auto result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@.}",
        ActivePath{ L"C:\\Windows\\System32" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Windows\\System32");

    result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@.}",
        ActivePath{ L"C:\\Windows\\System32\\ntdll.dll" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Windows\\System32");

    result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@..}",
        ActivePath{ L"C:\\Windows\\System32\\ntdll.dll" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Windows");

    result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@...}",
        ActivePath{ L"C:\\Windows\\System32\\ntdll.dll" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\");

    result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@....}",
        ActivePath{ L"C:\\Windows\\System32\\ntdll.dll" },
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\");
}