#include "module/user_defined/user_defined_module.h"
#include <Windows.h>
#include <shlobj_core.h>
#include "module/user_defined/bundle_parser.h"
#include "module/user_defined/user_defined_command.h"

namespace ra::module::user_defined {
namespace {

std::filesystem::path GetBundleDirectoryPath() {

    wchar_t* buffer{};
    HRESULT result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &buffer);
    if (FAILED(result)) {
        return {};
    }

    std::filesystem::path path{ buffer };
    CoTaskMemFree(buffer);

    return path / "RunAnywhere" / "Bundles";
}

}


UserDefinedModule::UserDefinedModule() : bundle_depot_(std::make_unique<BundleDepot>()) {

}


void UserDefinedModule::Reload() {

    bundle_depot_->Clear();

    auto bundle_directory_path = GetBundleDirectoryPath();

    try {

        for (std::filesystem::directory_iterator iterator(bundle_directory_path); 
             iterator != std::filesystem::directory_iterator(); 
             ++iterator) {

            if (iterator->path().extension() == ".rabdl") {
                LoadBundle(iterator->path());
            }
        }
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


std::shared_ptr<Bundle> UserDefinedModule::ParseImportedBundle(
    const std::filesystem::path& bundle_path,
    ImportBundleResult& result) {

    BundleParser parser(bundle_path);

    try {

        return parser.Parse();
    }
    catch (const zaf::Error&) {
        return nullptr;
    }
}


std::vector<CommandBrief> UserDefinedModule::QuerySuggestedCommands(
    const std::wstring& command_text) {

    auto found_entries = bundle_depot_->FindEntriesBeginWith(command_text);

    std::vector<CommandBrief> result;
    for (const auto& each_entry : found_entries) {
        result.emplace_back(each_entry->Keyword(), each_entry->Description());
    }

    return result;
}


std::shared_ptr<Command> UserDefinedModule::Interpret(const utility::CommandLine& command_line) {

    int argument_count{};
    auto arguments = CommandLineToArgvW(command_line.Text().c_str(), &argument_count);
    if (!arguments) {
        return nullptr;
    }

    std::shared_ptr<Command> command;
    if (argument_count > 0) {

        auto entry = bundle_depot_->FindEntry(arguments[0]);
        if (entry) {

            std::vector<std::wstring> command_arguments;
            for (int index = 1; index < argument_count; ++index) {
                command_arguments.push_back(arguments[index]);
            }

            command = std::make_shared<UserDefinedCommand>(entry, command_arguments);
        }
    }

    LocalFree(arguments);
    return command;
}

}