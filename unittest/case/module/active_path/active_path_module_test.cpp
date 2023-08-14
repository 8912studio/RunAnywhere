#include <gtest/gtest.h>
#include "module/active_path/active_path_module.h"

using namespace ra::context;
using namespace ra::mod::active_path;
using namespace ra::utility;

TEST(ActivePathModuleTest, CreateCommand) {

    DesktopContext context;
    context.active_path = ActivePath{ L"C:\\Windows\\System32\\svc.dll" };

    ActivePathModule module;

    {
        CommandLine command_line{ L"" };
        auto command = module.CreateCommand(command_line);
        ASSERT_EQ(command, nullptr);
    }

    {
        CommandLine command_line{ L"@" };
        auto command = module.CreateCommand(command_line);
        ASSERT_NE(command, nullptr);
        command->Interpret(command_line, context, false);
        ASSERT_EQ(command->GetPreviewText(), L"C:\\Windows\\System32\\svc.dll");
    }

    {
        CommandLine command_line{ L"@.. bla" };
        auto command = module.CreateCommand(command_line);
        ASSERT_NE(command, nullptr);
        command->Interpret(command_line, context, false);
        ASSERT_EQ(command->GetPreviewText(), L"C:\\Windows");
    }

    {
        CommandLine command_line{ L"\"@=C:\\Program Files\"" };
        auto command = module.CreateCommand(command_line);
        ASSERT_NE(command, nullptr);
        command->Interpret(command_line, context, false);
        ASSERT_EQ(command->GetPreviewText(), L"C:\\Program Files");
    }

    {
        CommandLine command_line(L"\ufffc", [](int) {
            return CommandLinePiece{ L"@=test abcd" };
        });
        auto command = module.CreateCommand(command_line);
        ASSERT_NE(command, nullptr);
        command->Interpret(command_line, context, false);
        ASSERT_EQ(command->GetPreviewText(), L"test abcd");
    }
}