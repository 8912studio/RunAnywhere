#include <gtest/gtest.h>
#include "module/user_defined/parse/entry_command_parsing.h"

using namespace ra::context;
using namespace ra::mod::user_defined;

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

    //Active path is in command part.
    result = ParseEntryCommand(
        L"{@}\\app.exe", 
        VariableFormatterFromActivePath(L"C:\\MyApp"),
        {});
    ASSERT_EQ(result.command, L"C:\\MyApp\\app.exe");
    ASSERT_TRUE(result.arguments.empty());
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


TEST(EntryCommandParsingTest, ReplaceVariableInArgument_SpaceIssues) {

    BundleMeta::Builder meta_builder;
    meta_builder.AddGlobalProperty(L"SpaceVariable", L"C:\\Program Files");
    meta_builder.AddGlobalProperty(L"NoSpaceVariable", L"C:\\ProgramFiles");

    VariableFormatter variable_formatter{ meta_builder.Build(), ActivePath{} };

    auto result = ParseEntryCommand(
        L"app.exe "
        L"{SpaceVariable!} "
        L"\"{SpaceVariable!}\" "
        L"{NoSpaceVariable!} " 
        L"\"{NoSpaceVariable!}\" "
        L"path={SpaceVariable!} "
        L"\"path={SpaceVariable!}\" "
        L"path={NoSpaceVariable!} "
        L"\"path={NoSpaceVariable!}\" "
        L"\"path = {SpaceVariable!}\" "
        L"\"path = {NoSpaceVariable!}\" ",
        variable_formatter, 
        {});

    ASSERT_EQ(result.arguments.size(), 10);
    ASSERT_EQ(result.arguments[0], L"\"C:\\Program Files\"");
    ASSERT_EQ(result.arguments[1], L"\"C:\\Program Files\"");
    ASSERT_EQ(result.arguments[2], L"C:\\ProgramFiles");
    ASSERT_EQ(result.arguments[3], L"C:\\ProgramFiles");
    ASSERT_EQ(result.arguments[4], L"path=\"C:\\Program Files\"");
    ASSERT_EQ(result.arguments[5], L"path=\"C:\\Program Files\"");
    ASSERT_EQ(result.arguments[6], L"path=C:\\ProgramFiles");
    ASSERT_EQ(result.arguments[7], L"path=C:\\ProgramFiles");
    ASSERT_EQ(result.arguments[8], L"\"path = C:\\Program Files\"");
    ASSERT_EQ(result.arguments[9], L"\"path = C:\\ProgramFiles\"");
}


TEST(EntryCommandParsingTest, ReplacePlaceholders) {

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
    
    //All placeholds have input arguments.
    auto result = ParseEntryCommand(
        L"C:\\Windows\\notepad.exe %1 %2 %3 %4 %5 %6 %7 %8 %9",
        variable_formatter,
        arguments);
    ASSERT_EQ(result.arguments, arguments);

    //Only some placeholders have input arguments.
    result = ParseEntryCommand(L"C:\\Windows\\notepad.exe %1 %2 %3", variable_formatter, {
        L"111",
    });
    ASSERT_EQ(result.arguments.size(), 1);
    ASSERT_EQ(result.arguments[0], L"111");

    //Space issue.
    result = ParseEntryCommand(L"C:\\Windows\\notepad.exe %1 \"%3\" \"%4\"", variable_formatter, {
        L"1 1 1",
        L"2",
        L"3 3",
        L"4444"
    });
    ASSERT_EQ(result.arguments.size(), 3);
    ASSERT_EQ(result.arguments[0], L"\"1 1 1\"");
    ASSERT_EQ(result.arguments[1], L"\"3 3\"");
    ASSERT_EQ(result.arguments[2], L"4444");

    //Placeholdr in command part.
    result = ParseEntryCommand(
        L"http://www.mysite.com/id=%1?type=page", 
        variable_formatter, 
        { L"9829" });
    ASSERT_EQ(result.command, L"http://www.mysite.com/id=9829?type=page");
    ASSERT_TRUE(result.arguments.empty());
}


TEST(EntryCommandParsingTest, MultipleLineInputArguments) {

    VariableFormatter variable_formatter{ std::make_shared<BundleMeta>(), ActivePath{} };

    auto result = ParseEntryCommand(
        L"C:\\Windows\\Explorer.exe %1 %2 %3", 
        variable_formatter,
        std::vector<std::wstring>{
            L"Argument1\rArgument1",
            L"Argument2\nArgument2",
            L"Argument3\r\nArgument3",
        }
    );

    ASSERT_EQ(result.command, L"C:\\Windows\\Explorer.exe");

    std::vector<std::wstring> expected{
        L"Argument1",
        L"Argument2",
        L"Argument3",
    };
    ASSERT_EQ(result.arguments, expected);
}
