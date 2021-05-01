#pragma once

#include <filesystem>

namespace ra::context {

class DesktopContext {
public:
    std::filesystem::path active_path;
};

}