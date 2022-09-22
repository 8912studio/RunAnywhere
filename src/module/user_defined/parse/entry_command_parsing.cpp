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

std::wstring GetPlaceholderText(
    const EntryCommandParameterPart& parameter_part,
    const std::vector<std::wstring>& input_arguments,
    bool auto_quote) {

    if (parameter_part.type != EntryCommandParameterPart::Type::General) {
        return std::wstring{};
    }

    std::size_t argument_index = parameter_part.general_index - 1;
    if (argument_index >= input_arguments.size()) {
        return std::wstring{};
    }

    const auto& argument = input_arguments[argument_index];
    if (auto_quote && zaf::Contain(argument, L' ')) {
        return L'"' + argument + L'"';
    }

    return argument;
}


std::wstring ReplacePlaceholders(
    std::wstring_view parameter_string,
    const std::vector<std::wstring>& input_arguments,
    bool auto_quote) {

    auto parsed_parts = ParseEntryCommandParameter(parameter_string);

    std::wstring result;
    std::size_t current_position{};
    for (const auto& each_part : parsed_parts) {

        result += parameter_string.substr(
            current_position,
            each_part.position - current_position);

        result += GetPlaceholderText(each_part, input_arguments, auto_quote);

        current_position = each_part.position + each_part.length;
    }

    if (current_position < parameter_string.length()) {
        result += parameter_string.substr(current_position);
    }

    return result;
}


std::vector<std::wstring> BuildArguments(
    const std::vector<std::wstring>& entry_parameters,
    const VariableFormatter& variable_formatter,
    const std::vector<std::wstring>& input_arguments) {

    std::vector<std::wstring> result;

    for (const auto& each_parameter : entry_parameters) {

        bool has_space = zaf::Contain(each_parameter, L' ');
        bool auto_quote = !has_space;

        //Format variables first.
        VariableFormatOptions format_options;
        format_options.auto_quote_variable = auto_quote;
        auto argument = variable_formatter.Format(each_parameter, format_options);

        //Replace placeholders then.
        argument = ReplacePlaceholders(argument, input_arguments, auto_quote);
        if (argument.empty()) {
            continue;
        }

        if (has_space) {
            argument = L'"' + argument + L'"';
        }

        result.push_back(argument);
    }
    
    return result;
}

}

CommandLineInfo ParseEntryCommand(
    const std::wstring& entry_command,
    const VariableFormatter& variable_formatter,
    const std::vector<std::wstring>& input_arguments) {

    utility::CommandLine command_line{ entry_command };

    CommandLineInfo result;
    result.command = variable_formatter.Format(command_line.Command(), VariableFormatOptions{});
    result.arguments = BuildArguments(
        command_line.Arguments(), 
        variable_formatter,
        input_arguments);

    return result;
}

}