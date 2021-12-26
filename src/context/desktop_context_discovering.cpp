#include "context/desktop_context_discovering.h"
#include <Windows.h>
#include "context/everything_discovering.h"
#include "context/explorer_discovering.h"
#include "context/vs_discovering.h"

namespace ra::context {
namespace {

ActivePath DiscoverActivePath(HWND foreground_window) {

    auto result = DiscoverActivePathFromEverything(foreground_window);
    if (!result.IsEmpty()) {
        return result;
    }

    result = DiscoverActivePathFromVS(foreground_window);
    if (!result.IsEmpty()) {
        return result;
    }

    return DiscoverActivePathFromExplorer(foreground_window);
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