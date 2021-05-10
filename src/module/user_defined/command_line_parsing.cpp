#include "module/user_defined/command_line_parsing.h"
#include <Windows.h>
#include <filesystem>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/string/replace.h>
#include "module/active_path/active_path_modifying.h"
#include "module/user_defined/command_parameter_parsing.h"

namespace ra::module::user_defined {
namespace {

std::wstring BuildActivePathArgument(
    std::wstring_view parameter_string,
    const CommandParameter& parameter,
    const context::ActivePath& active_path) {

    if (active_path.IsEmpty()) {
        return {};
    }

    auto new_active_path = active_path::ModifyActivePathByOption(
        active_path, 
        parameter.active_path_option);

    auto path = new_active_path.GetPath().wstring();
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
    const context::ActivePath& active_path,
    const std::vector<std::wstring>& input_arguments) {

    if (parameter.type == CommandParameter::Type::ActivePath) {
        return BuildActivePathArgument(parameter_string, parameter, active_path);
    }

    return {};
}


std::wstring BuildArgument(
    std::wstring_view parameter_string,
    const context::ActivePath& active_path,
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
    const context::ActivePath& active_path,
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
    const context::ActivePath& active_path,
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

}