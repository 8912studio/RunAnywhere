#pragma once

#include <Windows.h>
#include <filesystem>

std::filesystem::path DiscoverFocusedPathFromVS(HWND foreground_window_handle);