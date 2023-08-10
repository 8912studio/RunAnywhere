#include <gtest/gtest.h>
#include "module/active_path/active_path_modifying.h"

using namespace ra::context;
using namespace ra::mod::active_path;

TEST(ActivePathModifyingTest, ModifyByOverridingPath) {

	ActivePath path{ L"C:\\Windows\\System32\\ntdll.dll", L"C:\\Windows" };

	ActivePathOption option;
	option.overriding_path = L"D:\\Program Files\\Test";
	auto new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"D:\\Program Files\\Test");
	ASSERT_EQ(new_path.GetWorkspacePath(), L"C:\\Windows");
}


TEST(ActivePathModifyingTest, ModifyFilePathByBackwardLevel) {

	ActivePath path{ L"C:\\Windows\\System32\\ntdll.dll" };

	ActivePathOption option;
	option.backward_level = 0;
	auto new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"C:\\Windows\\System32\\ntdll.dll");

	option.backward_level = 1;
	new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"C:\\Windows\\System32");

	option.backward_level = 2;
	new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"C:\\Windows");
}


TEST(ActivePathModifyingTest, ModifyDirectoryPathByBackwardLevel) {

	ActivePath path{ L"C:\\Windows\\System32" };

	ActivePathOption option;
	option.backward_level = 0;
	auto new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"C:\\Windows\\System32");

	option.backward_level = 1;
	new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"C:\\Windows\\System32");

	option.backward_level = 2;
	new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"C:\\Windows");
}


TEST(ActivePathModifyingTest, ModifyByUseName) {

	ActivePath path{ L"C:\\Windows\\System32\\ntdll.dll" };

	ActivePathOption option;
	option.use_name = true;
	auto new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"ntdll.dll");
}


TEST(ActivePathModifyingTest, ModifyByUseWorkspacePath) {

	ActivePath path{ L"C:\\Windows\\System32\\ntdll.dll", L"C:\\Windows" };

	ActivePathOption option;
	option.use_workspace_path = true;
	auto new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"C:\\Windows");
	ASSERT_EQ(new_path.GetWorkspacePath(), L"C:\\Windows");

	option.use_workspace_path = false;
	new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"C:\\Windows\\System32\\ntdll.dll");
	ASSERT_EQ(new_path.GetWorkspacePath(), L"C:\\Windows");

	//Use workspace path, but there is no workspace path.
	path = ActivePath{ L"C:\\Windows\\System32\\ntdll.dll" };
	option.use_workspace_path = true;
	new_path = ModifyActivePathByOption(path, option);
	ASSERT_EQ(new_path.GetPath(), L"C:\\Windows\\System32\\ntdll.dll");
	ASSERT_EQ(new_path.GetWorkspacePath(), L"");
}


TEST(ActivePathModifyingTest, ModifyByCombinedOptions) {

	ActivePath path{ L"C:\\Windows\\System32\\ntdll.dll", L"C:\\Windows\\System32" };

	{
		ActivePathOption option;
		option.use_name = true;
		option.backward_level = 2;
		auto new_path = ModifyActivePathByOption(path, option);
		ASSERT_EQ(new_path.GetPath(), L"Windows");
		ASSERT_EQ(new_path.GetWorkspacePath(), L"C:\\Windows\\System32");
	}

	{
		ActivePathOption option;
		option.use_workspace_path = true;
		option.use_name = true;
		option.backward_level = 1;
		auto new_path = ModifyActivePathByOption(path, option);
		ASSERT_EQ(new_path.GetPath(), L"System32");
		ASSERT_EQ(new_path.GetWorkspacePath(), L"C:\\Windows\\System32");
	}

	{
		ActivePathOption option;
		option.overriding_path = L"D:\\Windows\\etc";
		option.use_name = true;
		option.use_workspace_path = true;
		option.backward_level = 2;
		auto new_path = ModifyActivePathByOption(path, option);
		ASSERT_EQ(new_path.GetPath(), L"D:\\Windows\\etc");
		ASSERT_EQ(new_path.GetWorkspacePath(), L"C:\\Windows\\System32");
	}
}