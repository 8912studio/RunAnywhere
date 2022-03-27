#pragma once

#include <filesystem>
#include "module/extension/extension_module.h"

namespace ra::module::extension {

class ExtensionModuleManager {
public:
    ExtensionModuleManager(const std::filesystem::path& extension_directory_path);

    void Load();

    const std::vector<std::shared_ptr<ExtensionModule>>& GetAllModules() const {
        return modules_;
    }

private:
    std::filesystem::path extension_directory_path_;
    std::vector<std::shared_ptr<ExtensionModule>> modules_;
};

}