#include <gtest/gtest.h>
#include "module/tool/tool_module.h"

using namespace ra::module::tool;
using namespace ra::utility;

TEST(ToolModuleTest, InterpretDateCommand) {

    auto tool_module = std::make_shared<ToolModule>();

	auto command = tool_module->Interpret(CommandLine{ L"date" });
	ASSERT_NE(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"datetime" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"dat" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"data" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"Date" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"DATE" });
	ASSERT_EQ(command, nullptr);
}


TEST(ToolModuleTest, InterpretMD5Command) {

	auto tool_module = std::make_shared<ToolModule>();

	auto command = tool_module->Interpret(CommandLine{ L"md5" });
	ASSERT_NE(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"MD5" });
	ASSERT_EQ(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"Md5" });
	ASSERT_EQ(command, nullptr);
}


TEST(ToolModuleTest, InterpretRGBCommand) {

	auto tool_module = std::make_shared<ToolModule>();

	auto command = tool_module->Interpret(CommandLine{ L"rgb" });
	ASSERT_NE(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"RGB" });
	ASSERT_EQ(command, nullptr);
}


TEST(ToolModuleTest, InterpretHexCommand) {

	auto tool_module = std::make_shared<ToolModule>();

	auto command = tool_module->Interpret(CommandLine{ L"hex"});
	ASSERT_NE(command, nullptr);

	command = tool_module->Interpret(CommandLine{ L"Hex" });
	ASSERT_EQ(command, nullptr);
}