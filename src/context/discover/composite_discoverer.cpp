#include "context/discover/composite_discoverer.h"

namespace ra::context {

CompositeDiscoverer::CompositeDiscoverer(
    std::vector<std::unique_ptr<Discoverer>> discoverers) noexcept 
    :
    discoverers_(std::move(discoverers)) {

}


std::optional<ActivePath> CompositeDiscoverer::Discover(
    const ForegroundWindowInfo& foreground_window_info) {

    for (const auto& each_discoverer : discoverers_) {

        auto active_path = each_discoverer->Discover(foreground_window_info);
        if (active_path) {
            return active_path;
        }
    }

    return {};
}

}