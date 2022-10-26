#pragma once

#include <Windows.h>
#include <zaf/base/non_copyable.h>
#include "context/active_path.h"

namespace ra::context {

class ForegroundWindowInfo {
public:
    HWND window_handle{};
    DWORD process_id{};
};


class Discoverer : zaf::NonCopyable {
public:
    virtual ActivePath Discover(const ForegroundWindowInfo& foreground_window_info) = 0;
};

}