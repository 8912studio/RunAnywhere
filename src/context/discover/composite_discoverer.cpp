#include "context/discover/composite_discoverer.h"
#include "context/discover/everything_discoverer.h"
#include "context/discover/explorer_discoverer.h"
#include "context/discover/window_based_discoverer.h"

namespace ra::context {

ActivePath CompositeDiscoverer::Discover(HWND foreground_window_handle) {

    TryToInitializeDiscoverers();

    for (const auto& each_discoverer : discoverers_) {

        auto active_path = each_discoverer->Discover(foreground_window_handle);
        if (!active_path.IsEmpty()) {
            return active_path;
        }
    }

    return {};
}


void CompositeDiscoverer::TryToInitializeDiscoverers() {

    if (!discoverers_.empty()) {
        return;
    }

    discoverers_.push_back(std::make_unique<ExplorerDiscoverer>());
    discoverers_.push_back(std::make_unique<EverythingDiscoverer>());
    discoverers_.push_back(std::make_unique<WindowBasedDiscoverer>());
}

}