#pragma once

#include <Windows.h>
#include <filesystem>

namespace ra::context {

std::filesystem::path DiscoverFocusedPathFromExplorer(HWND foreground_window_handle);

}