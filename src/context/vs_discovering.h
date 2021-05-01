#pragma once

#include <Windows.h>
#include <filesystem>

namespace ra::context {

std::filesystem::path DiscoverFocusedPathFromVS(HWND foreground_window_handle);

}