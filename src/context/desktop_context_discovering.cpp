#include "context/desktop_context_discovering.h"
#include <Windows.h>
#include "context/explorer_discovering.h"
#include "context/vs_discovering.h"

DesktopContext DiscoverDesktopContext() {

    HWND foreground_window = GetForegroundWindow();

    DesktopContext result;
    result.current_focused_path = DiscoverFocusedPathFromVS(foreground_window);

    if (result.current_focused_path.empty()) {
        result.current_focused_path = DiscoverFocusedPathFromExplorer(foreground_window);
    }

    return result;
}