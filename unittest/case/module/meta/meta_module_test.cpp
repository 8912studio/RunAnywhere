#include <gtest/gtest.h>
#include "module/meta/meta_module.h"

using namespace ra::mod::meta;
using namespace ra::utility;

TEST(MetaModuleTest, QuerySuggestedCommands) {

    MetaModule meta_module;

    {
        auto result = meta_module.QuerySuggestedCommands(L"");
        ASSERT_FALSE(result.empty());
    }

    {
        auto result = meta_module.QuerySuggestedCommands(L"a");
        ASSERT_TRUE(result.empty());
    }

    {
        auto result = meta_module.QuerySuggestedCommands(L"!");
        ASSERT_FALSE(result.empty());
    }

    {
        auto result = meta_module.QuerySuggestedCommands(L"!ab");
        ASSERT_FALSE(result.empty());
    }

    {
        auto result = meta_module.QuerySuggestedCommands(L"!abdddddddddddd");
        ASSERT_TRUE(result.empty());
    }
}


TEST(MetaModuleTest, CreateCommand) {

    MetaModule meta_module;

    {
        CommandLine command_line{ L"!about" };
        auto command = meta_module.CreateCommand(command_line);
        ASSERT_NE(command, nullptr);
    }

    {
        CommandLine command_line{ L"!option" };
        auto command = meta_module.CreateCommand(command_line);
        ASSERT_NE(command, nullptr);
    }

    {
        CommandLine command_line{ L"!reload" };
        auto command = meta_module.CreateCommand(command_line);
        ASSERT_NE(command, nullptr);
    }

    {
        CommandLine command_line{ L"reload" };
        auto command = meta_module.CreateCommand(command_line);
        ASSERT_EQ(command, nullptr);
    }

    {
        CommandLine command_line{ L"!nothing" };
        auto command = meta_module.CreateCommand(command_line);
        ASSERT_EQ(command, nullptr);
    }

    {
        CommandLine command_line(L"\ufffc", [](int) {
            return CommandLinePiece(CommandLinePieceType::TextBlock, L"!about");
        });
        auto command = meta_module.CreateCommand(command_line);
        ASSERT_EQ(command, nullptr);
    }
}