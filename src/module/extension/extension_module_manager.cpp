#include "module/extension/extension_module_manager.h"
#include <zaf/base/string/case_conversion.h>

namespace ra::mod::extension {

ExtensionModuleManager::ExtensionModuleManager(
    const std::filesystem::path& extension_directory_path)
    :
    extension_directory_path_(extension_directory_path) {

}


void ExtensionModuleManager::Load() {

    modules_.clear();

    try {

        for (std::filesystem::directory_iterator iterator{ extension_directory_path_ };
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
            modules_.push_back(extension_module);
        }
    }
    catch (const std::filesystem::filesystem_error&) {

    }
}

}