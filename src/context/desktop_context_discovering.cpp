#include "context/desktop_context_discovering.h"
#include <Windows.h>
#include "context/discover/composite_discoverer.h"

namespace ra::context {
namespace {

ActivePath DiscoverActivePath(const ForegroundWindowInfo& foreground_window_info) {

    static CompositeDiscoverer discoverer;
    return discoverer.Discover(foreground_window_info);
}

}

DesktopContext DiscoverDesktopContext() {

    ForegroundWindowInfo foreground_window_info;
    foreground_window_info.window_handle = GetForegroundWindow();
    if (!foreground_window_info.window_handle) {
        return {};
    }

    GetWindowThreadProcessId(
        foreground_window_info.window_handle, 
        &foreground_window_info.process_id);
    
    DesktopContext result;
    result.active_path = DiscoverActivePath(foreground_window_info);
    return result;
}

}