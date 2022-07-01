#pragma once

#include <optional>
#include <vector>
#include "module/module.h"
#include "module/user_defined/bundle_depot.h"
#include "module/user_defined/import_bundle_options.h"
#include "module/user_defined/import_bundle_result.h"
#include "module/user_defined/bundle.h"
#include "module/user_defined/entry.h"

namespace ra::module::user_defined {

class UserDefinedModule : public Module {
public:
    UserDefinedModule();

    void Reload();

    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;
    std::shared_ptr<Command> Interpret(const utility::CommandLine& command_line) override;

private:
    void LoadBundle(const std::filesystem::path& bundle_path);
    std::shared_ptr<Bundle> ParseImportedBundle(
        const std::filesystem::path& bundle_path,
        ImportBundleResult& result);

private:
    std::unique_ptr<BundleDepot> bundle_depot_;
};

}