#include "module/user_defined/legacy/legacy_entry_upgrading.h"
#include <fstream>
#include "module/user_defined/legacy/legacy_entry_reading.h"
#include "module/user_defined/parse/entry_command_placeholder_parsing.h"

namespace ra::module::user_defined {
namespace {

std::wstring ReplaceLegacyActivePathVariable(const std::wstring& legacy_command_line) {

    std::wstring result;

    std::size_t current_index{};
    while (current_index < legacy_command_line.size()) {

        auto placeholder_begin_index = legacy_command_line.find(L'%', current_index);
        if (placeholder_begin_index == std::wstring::npos) {
            break;
        }

        result += legacy_command_line.substr(
            current_index, 
            placeholder_begin_index - current_index);

        auto placeholder_end_index = legacy_command_line.find(L'@', placeholder_begin_index);
        if (placeholder_begin_index == std::wstring::npos) {
            break;
        }

        auto modifier = legacy_command_line.substr(
            placeholder_begin_index + 1, 
            placeholder_end_index - placeholder_begin_index - 1);

        result += L"{@" + modifier + L"}";

        current_index = placeholder_end_index + 1;
    }

    if (current_index < legacy_command_line.size()) {
        result += legacy_command_line.substr(current_index);
    }

    return result;
}

}


bool UpgradeLegacyEntries(
    const std::filesystem::path& legacy_file_path,
    const std::filesystem::path& bundle_file_path) {

    auto legacy_entries = ReadUserDefinedEntries(legacy_file_path);
    if (legacy_entries.empty()) {
        return false;
    }

    std::wofstream bundle_file_stream(bundle_file_path, std::wofstream::trunc);
    if (!bundle_file_stream) {
        return false;
    }

    for (const auto& each_entry : legacy_entries) {

        bundle_file_stream << L"[" << each_entry.keyword << L"]\n";

        auto replaced_command_line = ReplaceLegacyActivePathVariable(each_entry.command_line);

        bundle_file_stream << L"Command=" << replaced_command_line << L"\n\n";
    }

    return true;
}

}