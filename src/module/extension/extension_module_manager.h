#pragma once

#include <filesystem>
#include <zaf/base/non_copyable.h>
#include "module/extension/extension_module.h"

namespace ra::mod::extension {

class ExtensionModuleManager : zaf::NonCopyableNonMovable {
public:
    ExtensionModuleManager() = default;

    void Load(const std::vector<std::filesystem::path>& directory_paths);

    const std::vector<std::shared_ptr<ExtensionModule>>& GetAllModules() const {
        return modules_;
    }

private:
    void LoadModulesInDirectory(const std::filesystem::path& directory_path);

private:
    std::vector<std::shared_ptr<ExtensionModule>> modules_;
};

}