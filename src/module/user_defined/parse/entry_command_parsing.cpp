#include "module/user_defined/parse/entry_command_parsing.h"
#include <Windows.h>
#include <filesystem>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/string/replace.h>
#include "module/active_path/active_path_modifying.h"
#include "module/user_defined/parse/entry_command_parameter_parsing.h"
#include "utility/command_line.h"

namespace ra::module::user_defined {
namespace {

std::wstring BuildArgumentFromParameter(
    std::wstring_view parameter_string,
    const EntryCommandParameterPart& parameter_part,
    const context::ActivePath& active_path,
    const std::vector<std::wstring>& input_arguments) {

    return {};
}


std::wstring BuildArgument(
    std::wstring_view parameter_string,
    const context::ActivePath& active_path,
    const std::vector<std::wstring>& input_arguments) {

    auto parsed_parts = ParseEntryCommandParameter(parameter_string);

    std::wstring result;
    std::size_t current_position{};
    for (const auto& each_part : parsed_parts) {

        result += parameter_string.substr(
            current_position,
            each_part.position - current_position);

        result += BuildArgumentFromParameter(
            parameter_string,
            each_part,
            active_path,
            input_arguments);

        current_position = each_part.position + each_part.length;
    }

    if (current_position < parameter_string.length()) {
        result += parameter_string.substr(current_position);
    }

    return result;
}


std::vector<std::wstring> BuildArguments(
    const std::vector<std::wstring>& entry_parameters,
    const context::ActivePath& active_path,
    const std::vector<std::wstring>& input_arguments) {

    std::vector<std::wstring> result;

    for (const auto& each_parameter : entry_parameters) {

        auto argument = BuildArgument(each_parameter, active_path, input_arguments);
        if (!argument.empty()) {
            result.push_back(argument);
        }
    }

    return result;
}

}

EntryCommandParseResult ParseEntryCommand(
    const std::wstring& entry_command,
    const context::ActivePath& active_path,
    const std::vector<std::wstring>& input_arguments) {

    utility::CommandLine command_line{ entry_command };

    EntryCommandParseResult result;
    result.command = command_line.Command();
    result.arguments = BuildArguments(command_line.Arguments(), active_path, input_arguments);
    return result;
}

}