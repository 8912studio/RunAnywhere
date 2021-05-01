#include "context/desktop_context_discovering.h"
#include <Windows.h>
#include "context/explorer_discovering.h"
#include "context/vs_discovering.h"

namespace ra::context {

DesktopContext DiscoverDesktopContext() {

    HWND foreground_window = GetForegroundWindow();

    DesktopContext result;
    result.active_path = DiscoverFocusedPathFromVS(foreground_window);

    if (result.active_path.empty()) {
        result.active_path = DiscoverFocusedPathFromExplorer(foreground_window);
    }

    return result;
}

}