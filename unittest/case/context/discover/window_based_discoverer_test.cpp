#include <gtest/gtest.h>
#include "context/discover/window_based_discoverer.h"

using namespace ra::context;

TEST(WindowBasedDiscovererTest, DecodeActivePath) {

	auto path = WindowBasedDiscoverer::DecodeActivePath(L"");
	ASSERT_TRUE(path.IsEmpty());

	path = WindowBasedDiscoverer::DecodeActivePath(L"|");
	ASSERT_TRUE(path.IsEmpty());

	path = WindowBasedDiscoverer::DecodeActivePath(L"C:\\Windows");
	ASSERT_EQ(path.GetPath(), L"C:\\Windows");
	ASSERT_FALSE(path.HasWorkspacePath());

	path = WindowBasedDiscoverer::DecodeActivePath(L"C:\\Windows|");
	ASSERT_EQ(path.GetPath(), L"C:\\Windows");
	ASSERT_FALSE(path.HasWorkspacePath());

	path = WindowBasedDiscoverer::DecodeActivePath(L"C:\\Windows|D:\\Programs");
	ASSERT_EQ(path.GetPath(), L"C:\\Windows");
	ASSERT_EQ(path.GetWorkspacePath(), L"D:\\Programs");

	path = WindowBasedDiscoverer::DecodeActivePath(L"|D:\\Programs");
	ASSERT_TRUE(path.IsEmpty());
	ASSERT_FALSE(path.HasWorkspacePath());
}