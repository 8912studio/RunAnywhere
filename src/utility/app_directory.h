#pragma once

#include <filesystem>

namespace ra::utility {

std::filesystem::path GetUserDataDirectoryPath();
std::filesystem::path GetAppDataDirectoryPath();

}