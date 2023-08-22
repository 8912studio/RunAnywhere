#include "module/user_defined/user_defined_module.h"
#include <Windows.h>
#include "module/user_defined/bundle_definition.h"
#include "module/user_defined/bundle_parser.h"
#include "module/user_defined/legacy/legacy_entry_file.h"
#include "module/user_defined/legacy/legacy_entry_upgrading.h"
#include "module/user_defined/user_defined_command.h"
#include "utility/data_directory.h"

namespace ra::mod::user_defined {
namespace {

std::filesystem::path GetBundleDirectoryPath() {
    return utility::GetDataDirectoryPath() / "Bundles";
}

}


UserDefinedModule::UserDefinedModule() {

}


void UserDefinedModule::Reload() {

    bundle_directory_path_ = GetBundleDirectoryPath();

    TryToUpgradeLegacyEntries();

    bundle_depot_ = std::make_shared<BundleDepot>();

    try {

        for (std::filesystem::directory_iterator iterator(bundle_directory_path_);
             iterator != std::filesystem::directory_iterator(); 
             ++iterator) {

            if (iterator->path().extension() == BundleFileExtension) {
                LoadBundle(iterator->path());
            }
        }
    }
    catch (const std::filesystem::filesystem_error&) {

    }
}


void UserDefinedModule::TryToUpgradeLegacyEntries() {

    try {

        auto legacy_entry_file_path = GetLegacyEntryFilePath();
        if (!std::filesystem::exists(legacy_entry_file_path)) {
            return;
        }

        auto default_bundle_file_path = bundle_directory_path_ / L"Default.ra-bundle";
        if (std::filesystem::exists(default_bundle_file_path)) {
            return;
        }

        std::filesystem::create_directories(bundle_directory_path_);

        if (!UpgradeLegacyEntries(legacy_entry_file_path, default_bundle_file_path)) {
            return;
        }

        std::filesystem::rename(
            legacy_entry_file_path,
            legacy_entry_file_path.parent_path() / L"UserDefined-Legacy.txt");
    }
    catch (const std::filesystem::filesystem_error&) {

    }
}


void UserDefinedModule::LoadBundle(const std::filesystem::path& bundle_path) {

    BundleParser parser(bundle_path);

    try {

        auto bundle = parser.Parse();
        bundle_depot_->AddBundle(bundle);
    }
    catch (const zaf::Error&) {

    }
}


std::shared_ptr<BundleImporter> UserDefinedModule::BeginImportBundle(
    const std::filesystem::path& bundle_path) {

    return std::make_shared<BundleImporter>(
        bundle_depot_,
        bundle_directory_path_,
        bundle_path);
}


std::vector<CommandBrief> UserDefinedModule::QuerySuggestedCommands(
    const std::wstring& command_text) {

    ZAF_EXPECT(bundle_depot_);

    auto found_entries = bundle_depot_->FindEntriesBeginWith(command_text);

    std::vector<CommandBrief> result;
    for (const auto& each_entry : found_entries) {
        result.emplace_back(each_entry->Keyword(), each_entry->Description());
    }

    return result;
}


std::unique_ptr<Command> UserDefinedModule::CreateCommand(
    const utility::CommandLine& command_line) {

    ZAF_EXPECT(bundle_depot_);

    auto entry = bundle_depot_->FindEntry(command_line.Command());
    if (!entry) {
        return nullptr;
    }

    return std::make_unique<UserDefinedCommand>(entry, command_line.Arguments());
}

}