#pragma once

#include <Windows.h>
#include "context/active_path.h"

namespace ra::context {

ActivePath DiscoverActivePathFromEverything(HWND foreground_window_handle);

}