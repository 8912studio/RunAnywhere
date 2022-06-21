#include "module/user_defined/user_defined_module.h"
#include <Windows.h>
#include <shlobj_core.h>
#include "module/user_defined/user_defined_bundle_parser.h"
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

void UserDefinedModule::Reload() {

    bundles_.clear();

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

    UserDefinedBundleParser parser(bundle_path);

    try {

        auto bundle = parser.Parse();
        bundles_.push_back(bundle);
    }
    catch (const zaf::Error&) {

    }
}


std::vector<CommandBrief> UserDefinedModule::QuerySuggestedCommands(
    const std::wstring& command_text) {

    std::vector<CommandBrief> result;

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            if (each_entry->Keyword().find(command_text) == 0) {
                result.emplace_back(each_entry->Keyword(), each_entry->Description());
            }
        }
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

        auto entry = FindEntry(arguments[0]);
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


std::shared_ptr<UserDefinedEntry> UserDefinedModule::FindEntry(std::wstring_view keyword) {

    for (const auto& each_bundle : bundles_) {
        for (const auto& each_entry : each_bundle->Entries()) {

            if (each_entry->Keyword() == keyword) {
                return each_entry;
            }
        }
    }

    return nullptr;
}

}