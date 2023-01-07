#pragma once

#include <filesystem>

enum class VSVersionType {
    Unsupported,
    VS2019OrOlder,
    VS2022OrNewer,
};

class VSInstallationInfo {
public:
    VSVersionType version_type{ VSVersionType::Unsupported };
    std::filesystem::path vsix_installer_path;
};

const VSInstallationInfo& GetVSInstallationInfo();
