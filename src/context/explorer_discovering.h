#pragma once

#include <Windows.h>
#include <filesystem>

std::filesystem::path DiscoverFocusedPathFromExplorer(HWND foreground_window_handle);