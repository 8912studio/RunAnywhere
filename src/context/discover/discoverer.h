#pragma once

#include <Windows.h>
#include <optional>
#include <zaf/base/non_copyable.h>
#include "context/active_path.h"

namespace ra::context {

class ForegroundWindowInfo {
public:
    HWND window_handle{};
    DWORD process_id{};
};


class Discoverer : zaf::NonCopyableNonMovable {
public:
    virtual ~Discoverer() = default;

    virtual std::optional<ActivePath> Discover(
        const ForegroundWindowInfo& foreground_window_info) = 0;
};

}