#include "module/user_defined/command_line_parsing.h"
#include <Windows.h>
#include <filesystem>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/string/replace.h>
#include "module/user_defined/command_parameter_parsing.h"

namespace {

std::wstring BuildActivePathArgument(
    std::wstring_view parameter_string,
    const CommandParameter& parameter,
    const std::filesystem::path& active_path) {

    if (active_path.empty()) {
        return {};
    }

    auto path = GetBackwardedActivePath(active_path, parameter.backward_level);
    if (parameter.is_quoted) {
        return path;
    }

    if (!zaf::Contain(path, L' ')) {
        return path;
    }

    return L'"' + path + L'"';
}


std::wstring BuildArgumentFromParameter(
    std::wstring_view parameter_string,
    const CommandParameter& parameter,
    const std::filesystem::path& active_path,
    const std::vector<std::wstring>& input_arguments) {

    if (parameter.type == CommandParameter::Type::ActivePath) {
        return BuildActivePathArgument(parameter_string, parameter, active_path);
    }

    return {};
}


std::wstring BuildArgument(
    std::wstring_view parameter_string, 
    const std::filesystem::path& active_path,
    const std::vector<std::wstring>& input_arguments) {

    auto parsed_parameters = ParseParameter(parameter_string);

    std::wstring result;
    std::size_t current_position{};
    for (const auto& each_parameter : parsed_parameters) {

        result += parameter_string.substr(
            current_position, 
            each_parameter.position - current_position);

        result += BuildArgumentFromParameter(
            parameter_string, 
            each_parameter, 
            active_path, 
            input_arguments);

        current_position = each_parameter.position + each_parameter.length;
    }

    if (current_position < parameter_string.length()) {
        result += parameter_string.substr(current_position);
    }

    return result;
}


std::vector<std::wstring> BuildArguments(
    wchar_t** parameters,
    int parameter_count,
    const std::filesystem::path& active_path,
    const std::vector<std::wstring>& input_arguments) {

    std::vector<std::wstring> result;

    for (int index = 1; index < parameter_count; ++index) {

        auto argument = BuildArgument(parameters[index], active_path, input_arguments);
        if (!argument.empty()) {
            result.push_back(argument);
        }
    }

    return result;
}


}

ParseResult ParseCommandLine(
    const std::wstring& command_line,
    const std::filesystem::path& active_path,
    const std::vector<std::wstring>& input_arguments) {

    ParseResult result;

    int parameter_count{};
    auto parameters = CommandLineToArgvW(command_line.c_str(), &parameter_count);
    if (!parameters) {
        return result;
    }

    if (parameter_count > 0) {

        result.command = parameters[0];
        result.arguments = BuildArguments(
            parameters, 
            parameter_count, 
            active_path,
            input_arguments);
    }

    LocalFree(parameters);
    return result;
}


std::wstring GetBackwardedActivePath(
    const std::filesystem::path& active_path,
    std::size_t backward_level) {

    try {

        auto backwarded = active_path;

        if (backward_level >= 1) {
            if (!std::filesystem::is_directory(backwarded)) {
                backwarded = backwarded.parent_path();
            }
        }

        for (std::size_t level = 2; level <= backward_level; ++level) {
            backwarded = backwarded.parent_path();
        }

        return backwarded.wstring();
    }
    catch (const std::filesystem::filesystem_error&) {
        return {};
    }
}