#include <gtest/gtest.h>
#include "module/user_defined/parse/entry_command_parsing.h"

using namespace ra::context;
using namespace ra::module::user_defined;

namespace {

VariableFormatter VariableFormatterFromActivePath(const std::wstring& active_path) {
    return VariableFormatter{
        std::make_shared<BundleMeta>(),
        ActivePath{ active_path }
    };
}

}

TEST(EntryCommandParsingTest, ReplaceActivePath) {

    //Empty path
    auto result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe {@}",
        VariableFormatterFromActivePath({}),
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 0);

    //No space in path
    result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe {@}", 
        VariableFormatterFromActivePath(L"C:\\Documents\\file.txt"),
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Documents\\file.txt");

    //There is space in path
    result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe {@}",
        VariableFormatterFromActivePath(L"C:\\My Documents\\file.txt"),
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"\"C:\\My Documents\\file.txt\"");

    //Parameter is quoted
    result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe \"{@}\"", 
        VariableFormatterFromActivePath(L"C:\\My Documents\\file.txt"),
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"\"C:\\My Documents\\file.txt\"");

    //Active path is a part of parameter
    result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe /path:{@}", 
        VariableFormatterFromActivePath(L"C:\\file.txt"),
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"/path:C:\\file.txt");

    result = ParseEntryCommand(
        L"C:\\Window\\notepad.exe /path:{@}",
        VariableFormatterFromActivePath(L"C:\\my file.txt"),
        {});
    ASSERT_EQ(result.command, L"C:\\Window\\notepad.exe");
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"/path:\"C:\\my file.txt\"");
}


TEST(EntryCommandParsingTest, BackwardActivePath) {

    auto result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@.}",
        VariableFormatterFromActivePath(L"C:\\Windows\\System32"),
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Windows\\System32");

    result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@.}",
        VariableFormatterFromActivePath(L"C:\\Windows\\System32\\ntdll.dll"),
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Windows\\System32");

    result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@..}",
        VariableFormatterFromActivePath(L"C:\\Windows\\System32\\ntdll.dll"),
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\Windows");

    result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@...}",
        VariableFormatterFromActivePath(L"C:\\Windows\\System32\\ntdll.dll"),
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\");

    result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe {@....}",
        VariableFormatterFromActivePath(L"C:\\Windows\\System32\\ntdll.dll"),
        {});
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"C:\\");
}


TEST(EntryCommandParsingTest, ReplaceGeneralParameter) {

    VariableFormatter variable_formatter{
        std::make_shared<BundleMeta>(),
        ActivePath{}
    };

    std::vector<std::wstring> arguments{
        L"1A",  
        L"2B",
        L"3C",
        L"4D",
        L"5E",
        L"6F",
        L"7G",
        L"8H",
        L"9I",
    };
    
    auto result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe %1 %2 %3 %4 %5 %6 %7 %8 %9",
        variable_formatter,
        arguments);
    ASSERT_EQ(result.arguments, arguments);

    result = ParseEntryCommand(L"C:\\Windows\\notepad.exe %1 %2 %3", variable_formatter, {
        L"111",
    });
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"111");

    result = ParseEntryCommand(L"C:\\Windows\\notepad.exe %1 \"%3\"", variable_formatter, {
        L"1 1 1",
        L"2",
        L"3 3"
    });
    ASSERT_EQ(result.arguments.size(), 2);
    ASSERT_EQ(result.arguments[0], L"\"1 1 1\"");
    ASSERT_EQ(result.arguments[1], L"\"3 3\"");
}