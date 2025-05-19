#pragma once

#include <memory>
#include <vector>
#include "context/discover/discoverer.h"

namespace ra::context {

class CompositeDiscoverer : public Discoverer {
public:
    explicit CompositeDiscoverer(std::vector<std::unique_ptr<Discoverer>> discoverers) noexcept;

    std::optional<ActivePath> Discover(
        const ForegroundWindowInfo& foreground_window_info) override;

private:
    std::vector<std::unique_ptr<Discoverer>> discoverers_;
};

}