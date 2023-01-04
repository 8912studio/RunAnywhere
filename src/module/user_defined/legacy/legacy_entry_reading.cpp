#include "module/user_defined/legacy/legacy_entry_reading.h"
#include <Windows.h>
#include <shlobj_core.h>
#include <fstream>
#include <optional>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>

namespace ra::module::user_defined {

std::vector<LegacyEntry> ReadUserDefinedEntries(const std::filesystem::path& file_path) {

    std::ifstream file_stream(file_path, std::ios::in);
    if (!file_stream) {
        return {};
    }

    std::vector<LegacyEntry> result;

    std::string line;
    while (std::getline(file_stream, line)) {

        zaf::Trim(line);

        //Ignore empty line.
        if (line.empty()) {
            continue;
        }

        //Ignore comment line.
        if (line.front() == '#') {
            continue;
        }

        //Read command line.
        std::string command_line;
        std::getline(file_stream, command_line);

        zaf::Trim(command_line);
        if (command_line.empty()) {
            continue;
        }

        LegacyEntry entry;
        entry.keyword = zaf::FromUTF8String(line);
        entry.command_line = zaf::FromUTF8String(command_line);
        result.push_back(std::move(entry));
    }

    return result;
}

}