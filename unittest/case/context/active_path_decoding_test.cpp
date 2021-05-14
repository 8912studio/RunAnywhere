#include <gtest/gtest.h>
#include "context/active_path_decoding.h"

using namespace ra::context;

TEST(ActivePathDecodingTest, Decode) {

	auto path = DecodeActivePath(L"");
	ASSERT_TRUE(path.IsEmpty());

	path = DecodeActivePath(L"|");
	ASSERT_TRUE(path.IsEmpty());

	path = DecodeActivePath(L"C:\\Windows");
	ASSERT_EQ(path.GetPath(), L"C:\\Windows");
	ASSERT_FALSE(path.HasWorkspacePath());

	path = DecodeActivePath(L"C:\\Windows|");
	ASSERT_EQ(path.GetPath(), L"C:\\Windows");
	ASSERT_FALSE(path.HasWorkspacePath());

	path = DecodeActivePath(L"C:\\Windows|D:\\Programs");
	ASSERT_EQ(path.GetPath(), L"C:\\Windows");
	ASSERT_EQ(path.GetWorkspacePath(), L"D:\\Programs");

	path = DecodeActivePath(L"|D:\\Programs");
	ASSERT_TRUE(path.IsEmpty());
	ASSERT_FALSE(path.HasWorkspacePath());
}