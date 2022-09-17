#include "context/desktop_context_discovering.h"
#include <Windows.h>
#include "context/discover/composite_discoverer.h"

namespace ra::context {
namespace {

ActivePath DiscoverActivePath(HWND foreground_window) {

    static auto discoverer = std::make_unique<CompositeDiscoverer>();
    return discoverer->Discover(foreground_window);
}

}

DesktopContext DiscoverDesktopContext() {

    DesktopContext result;

    HWND foreground_window = GetForegroundWindow();
    if (foreground_window) {
        result.active_path = DiscoverActivePath(foreground_window);
    }

    return result;
}

}