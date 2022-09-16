#pragma once

#include "context/discoverer/discoverer.h"

namespace ra::context {

class NPPDiscoverer : public Discoverer {
public:
    ActivePath Discover(HWND foreground_window_handle) override;
};

}