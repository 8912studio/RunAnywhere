#include "module/user_defined/user_defined_entry_reading.h"
#include <filesystem>
#include <fstream>
#include <optional>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>
#include "module/user_defined/user_defined_file.h"

namespace ra::module::user_defined {

std::vector<UserDefinedEntryLegacy> ReadUserDefinedEntries() {

    auto file_path = GetUserDefinedFilePath();
    std::ifstream file_stream(file_path, std::ios::in);
    if (!file_stream) {
        return {};
    }

    std::vector<UserDefinedEntryLegacy> result;

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

        UserDefinedEntryLegacy entry;
        entry.keyword = zaf::FromUtf8String(line);
        entry.command_line = zaf::FromUtf8String(command_line);
        result.push_back(std::move(entry));
    }

    return result;
}

}