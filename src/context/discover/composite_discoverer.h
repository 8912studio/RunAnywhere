#pragma once

#include <memory>
#include <vector>
#include "context/discover/discoverer.h"

namespace ra::context {

class CompositeDiscoverer : public Discoverer {
public:
    std::optional<ActivePath> Discover(
        const ForegroundWindowInfo& foreground_window_info) override;

private:
    void TryToInitializeDiscoverers();

private:
    std::vector<std::unique_ptr<Discoverer>> discoverers_;
};

}