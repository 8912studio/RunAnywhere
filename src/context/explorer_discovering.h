#pragma once

#include <Windows.h>
#include <filesystem>

namespace ra::context {

std::filesystem::path DiscoverActivePathFromExplorer(HWND foreground_window_handle);

}