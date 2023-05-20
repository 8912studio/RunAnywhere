#include <gtest/gtest.h>
#include "module/tool/tool_module.h"

using namespace ra::module::tool;
using namespace ra::utility;

TEST(ToolModuleTest, CreateDateCommand) {

    auto tool_module = std::make_shared<ToolModule>();

	auto command = tool_module->CreateCommand(CommandLine{ L"date" });
	ASSERT_NE(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"datetime" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"dat" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"data" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"Date" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"DATE" });
	ASSERT_EQ(command, nullptr);
}


TEST(ToolModuleTest, CreateMD5Command) {

	auto tool_module = std::make_shared<ToolModule>();

	auto command = tool_module->CreateCommand(CommandLine{ L"md5" });
	ASSERT_NE(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"MD5" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"Md5" });
	ASSERT_EQ(command, nullptr);
}


TEST(ToolModuleTest, CreateRGBCommand) {

	auto tool_module = std::make_shared<ToolModule>();

	auto command = tool_module->CreateCommand(CommandLine{ L"rgb" });
	ASSERT_NE(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"RGB" });
	ASSERT_EQ(command, nullptr);
}


TEST(ToolModuleTest, CreateHexCommand) {

	auto tool_module = std::make_shared<ToolModule>();

	auto command = tool_module->CreateCommand(CommandLine{ L"hex"});
	ASSERT_NE(command, nullptr);

	command = tool_module->CreateCommand(CommandLine{ L"Hex" });
	ASSERT_EQ(command, nullptr);
}