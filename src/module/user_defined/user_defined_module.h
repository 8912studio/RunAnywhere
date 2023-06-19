#pragma once

#include <optional>
#include <vector>
#include "module/module.h"
#include "module/user_defined/bundle.h"
#include "module/user_defined/bundle_depot.h"
#include "module/user_defined/entry.h"
#include "module/user_defined/import/bundle_importer.h"

namespace ra::mod::user_defined {

class UserDefinedModule : public Module {
public:
    UserDefinedModule();

    void Reload();

    std::shared_ptr<BundleImporter> BeginImportBundle(const std::filesystem::path& bundle_path);

    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;
    std::unique_ptr<Command> CreateCommand(const utility::CommandLine& command_line) override;

private:
    void TryToUpgradeLegacyEntries();
    void LoadBundle(const std::filesystem::path& bundle_path);

private:
    std::filesystem::path bundle_directory_path_;
    std::shared_ptr<BundleDepot> bundle_depot_;
};

}