#pragma once

#include <filesystem>

class VSInstallationInfo {
public:
    bool is_vs2019_or_older_installed{};
    bool is_vs2022_or_newer_installed{};
    std::filesystem::path vsix_installer_path;
};

const VSInstallationInfo& GetVSInstallationInfo();
