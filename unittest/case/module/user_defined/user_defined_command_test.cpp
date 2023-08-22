#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "module/user_defined/user_defined_command.h"
#include "module/user_defined/preview/user_defined_command_preview_control.h"

using namespace ra::context;
using namespace ra::mod::user_defined;
using namespace ra::utility;

namespace {

std::shared_ptr<Entry> CreateEntry(const std::wstring& command) {

    BundleMeta::Builder meta_builder;
    meta_builder.SetBundleID(L"test-bundle");

    Entry::Builder entry_builder;
    entry_builder.SetBundleMeta(meta_builder.Build());
    entry_builder.SetKeyword(L"test");
    entry_builder.SetCommand(command);

    return entry_builder.Build();
}

}

//In this case, we test that the entry got from preview control is the same as which is passed into
//command.
TEST(UserDefinedCommand, CommandEntry) {

    auto entry = CreateEntry(L"run test");

    UserDefinedCommand command{ entry, {} };
    auto preview_control = zaf::As<UserDefinedCommandPreviewControl>(command.GetPreviewControl());
    ASSERT_NE(preview_control, nullptr);
    ASSERT_EQ(preview_control->GetCommandEntry(), entry);
}


TEST(UserDefinedCommand, ExecuteInfo) {

    auto test = [](
        const std::wstring& entry_command,
        const CommandLine& command_line,
        const DesktopContext& context,
        const ExecuteInfo& expected) {

        auto entry = CreateEntry(entry_command);
        UserDefinedCommand command{ entry, command_line.Arguments() };

        if (!command.Interpret(command_line, context, false)) {
            return false;
        }

        auto preview_control = zaf::As<UserDefinedCommandPreviewControl>(command.GetPreviewControl());
        if (!preview_control) {
            return false;
        }

        const auto& execute_info = preview_control->GetExecuteInfo();
        return 
            execute_info.command_line.command == expected.command_line.command &&
            execute_info.command_line.arguments == expected.command_line.arguments &&
            execute_info.working_directory == expected.working_directory;
    };

    //No active path.
    {
        ExecuteInfo expected;
        expected.command_line.command = L"run";
        ASSERT_TRUE(test(L"run {@}", CommandLine(L"run"), {}, expected));
    }

    //Has active path.
    {
        DesktopContext context;
        context.active_path = ActivePath{ L"D:\\Workspace" };
        ExecuteInfo expected;
        expected.command_line.command = L"run";
        expected.command_line.arguments.push_back(L"D:\\Workspace");
        ASSERT_TRUE(test(L"run {@}", CommandLine(L"run"), context, expected));
    }

    //Active path modifier
    {
        DesktopContext context;
        context.active_path = ActivePath{ L"D:\\Workspace\\RunAnywhere\\src\\main.cpp" };
        ExecuteInfo expected;
        expected.command_line.command = L"run";
        expected.command_line.arguments.push_back(L"D:\\Workspace\\RunAnywhere");
        ASSERT_TRUE(test(L"run {@}", CommandLine(L"run @.."), context, expected));
    }

    //Active path modifier is not the first argument.
    {
        DesktopContext context;
        context.active_path = ActivePath{ L"D:\\Workspace\\RunAnywhere\\src\\main.cpp" };
        ExecuteInfo expected;
        expected.command_line.command = L"run";
        expected.command_line.arguments.push_back(L"D:\\Workspace\\RunAnywhere\\src\\main.cpp");
        ASSERT_TRUE(test(L"run {@}", CommandLine(L"run test @.."), context, expected));
    }

    //Active path overriding object.
    {
        CommandLine command_line(L"run \ufffc", [](int) {
            return CommandLinePiece{
                CommandLinePieceType::ActivePath,
                L"C:\\Program Files\\RunAnywhere\\RunAnywhere.exe" 
            };
        });
        DesktopContext context;
        context.active_path = ActivePath{ L"D:\\Workspace\\RunAnywhere\\src\\main.cpp" };
        ExecuteInfo expected;
        expected.command_line.command = L"run";
        expected.command_line.arguments = { 
            L"\"C:\\Program Files\\RunAnywhere\\RunAnywhere.exe\"" 
        };
        ASSERT_TRUE(test(L"run {@}", command_line, context, expected));
    }

    //Text block object.
    {
        CommandLine command_line(L"run \ufffc", [](int) {
            return CommandLinePiece{
                CommandLinePieceType::TextBlock,
                L"@=C:\\Windows\\explorer.exe"
            };
        });
        DesktopContext context;
        context.active_path = ActivePath{ L"D:\\Workspace\\RunAnywhere\\src\\main.cpp" };
        ExecuteInfo expected;
        expected.command_line.command = L"run";
        expected.command_line.arguments = { 
            L"D:\\Workspace\\RunAnywhere\\src\\main.cpp",
            L"@=C:\\Windows\\explorer.exe",
        };
        ASSERT_TRUE(test(L"run {@} %1", command_line, context, expected));
    }
}