#include "context/desktop_context_discovering.h"
#include "context/explorer_discovering.h"

DesktopContext DiscoverDesktopContext() {

    DesktopContext result;
    result.current_focused_path = DiscoverFocusedPathFromExplorer();
    return result;
}