#pragma once

#include <memory>
#include <vector>
#include "context/discover/discoverer.h"

namespace ra::context {

class CompositeDiscoverer : public Discoverer {
public:
    ActivePath Discover(HWND foreground_window_handle) override;

private:
    void TryToInitializeDiscoverers();

private:
    std::vector<std::unique_ptr<Discoverer>> discoverers_;
};

}