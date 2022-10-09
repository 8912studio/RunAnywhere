#include "module/user_defined/parse/entry_command_parsing.h"
#include <Windows.h>
#include <filesystem>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/string/replace.h>
#include "module/active_path/active_path_modifying.h"
#include "module/user_defined/parse/entry_command_placeholder_parsing.h"
#include "utility/command_line.h"

namespace ra::module::user_defined {
namespace {

std::wstring GetPlaceholderText(
    const EntryCommandPlaceholder& placeholder,
    const std::vector<std::wstring>& input_arguments,
    bool auto_quote) {

    std::size_t argument_index = placeholder.index - 1;
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

    auto placeholders = ParseEntryCommandPlaceholders(parameter_string);

    std::wstring result;
    std::size_t current_position{};
    for (const auto& each_placeholder : placeholders) {

        result += parameter_string.substr(
            current_position,
            each_placeholder.position - current_position);

        result += GetPlaceholderText(each_placeholder, input_arguments, auto_quote);

        current_position = each_placeholder.position + each_placeholder.length;
    }

    if (current_position < parameter_string.length()) {
        result += parameter_string.substr(current_position);
    }

    return result;
}


std::wstring FormatOnePart(
    const std::wstring& input,
    bool is_argument,
    const VariableFormatter& variable_formatter,
    const std::vector<std::wstring>& input_arguments) {

    bool auto_quote_result{};
    bool auto_quote_replaced_part{};
    if (is_argument) {
        auto_quote_result = zaf::Contain(input, L' ');
        auto_quote_replaced_part = !auto_quote_result;
    }

    //Format variables first.
    VariableFormatOptions format_options;
    format_options.auto_quote_variable = auto_quote_replaced_part;
    auto result = variable_formatter.Format(input, format_options);

    //Replace placeholders then.
    result = ReplacePlaceholders(result, input_arguments, auto_quote_replaced_part);
    if (!result.empty()) {

        if (auto_quote_result) {
            result = L'"' + result + L'"';
        }
    }

    return result;
}


std::vector<std::wstring> BuildArguments(
    const std::vector<std::wstring>& entry_parameters,
    const VariableFormatter& variable_formatter,
    const std::vector<std::wstring>& input_arguments) {

    std::vector<std::wstring> result;

    for (const auto& each_parameter : entry_parameters) {

        auto argument = FormatOnePart(each_parameter, true, variable_formatter, input_arguments);
        if (!argument.empty()) {
            result.push_back(argument);
        }
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
    result.command = FormatOnePart(
        command_line.Command(),
        false,
        variable_formatter,
        input_arguments);

    result.arguments = BuildArguments(
        command_line.Arguments(), 
        variable_formatter,
        input_arguments);

    return result;
}

}