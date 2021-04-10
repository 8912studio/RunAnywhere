#pragma once

#include <filesystem>

class DesktopContext {
public:
    std::filesystem::path current_focused_path;
};