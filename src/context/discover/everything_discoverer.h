#pragma once

#include "context/discover/discoverer.h"

namespace ra::context {

class EverythingDiscoverer : public Discoverer {
public:
    std::optional<ActivePath> Discover(
        const ForegroundWindowInfo& foreground_window_info) override;
};

}