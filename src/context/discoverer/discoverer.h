#pragma once

#include <Windows.h>
#include <zaf/base/non_copyable.h>
#include "context/active_path.h"

namespace ra::context {

class Discoverer : zaf::NonCopyable {
public:
    virtual ActivePath Discover(HWND foreground_window_handle) = 0;
};

}