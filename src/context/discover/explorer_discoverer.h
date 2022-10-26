#pragma once

#include "context/discover/discoverer.h"

namespace ra::context {

class ExplorerDiscoverer : public Discoverer {
public:
    ActivePath Discover(const ForegroundWindowInfo& foreground_window_info) override;
};

}