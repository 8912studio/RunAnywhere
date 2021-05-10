#pragma once

#include <Windows.h>
#include <filesystem>
#include "context/active_path.h"

namespace ra::context {

ActivePath DiscoverActivePathFromExplorer(HWND foreground_window_handle);

}