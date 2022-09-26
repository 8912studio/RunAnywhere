#pragma once

#include "context/discover/discoverer.h"

namespace ra::context {

class ExplorerDiscoverer : public Discoverer {
public:
    ActivePath Discover(HWND foreground_window_handle) override;
};

}