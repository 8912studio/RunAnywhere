#include "module/extension/extension_module_manager.h"
#include <zaf/base/string/case_conversion.h>

namespace ra::mod::extension {

void ExtensionModuleManager::Load(const std::vector<std::filesystem::path>& directory_paths) {

    modules_.clear();

    for (const auto& each_path : directory_paths) {
        LoadModulesInDirectory(each_path);
    }
}


void ExtensionModuleManager::LoadModulesInDirectory(const std::filesystem::path& directory_path) {

    try {

        for (std::filesystem::directory_iterator iterator{ directory_path };
            iterator != std::filesystem::directory_iterator();
            ++iterator) {

            if (!iterator->is_regular_file()) {
                continue;
            }

            auto extension = zaf::ToLowercased(iterator->path().extension());
            if (extension != L".dll") {
                continue;
            }

            auto extension_module = std::make_shared<ExtensionModule>(iterator->path());
            modules_.push_back(std::move(extension_module));
        }
    }
    catch (const std::filesystem::filesystem_error&) {

    }
}

}