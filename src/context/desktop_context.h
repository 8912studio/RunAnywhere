#pragma once

#include <filesystem>
#include "context/active_path.h"

namespace ra::context {

class DesktopContext {
public:
    ActivePath active_path;
};

}