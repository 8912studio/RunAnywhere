#include "module/user_defined/interpret/variable_formatter.h"

namespace ra::module::user_defined {
namespace {

bool IsValidVariableChar(wchar_t ch) {
    return !!std::isalnum(ch);
}

}

std::wstring VariableFormatter::Format(std::wstring_view input) {

    std::wstring result;

    std::size_t index{};
    while (index < input.size()) {

        if (input[index] == L'{') {

            ++index;

            //Variable
            if (input[index] != L'{') {

                auto formatted_variable = FormatVariable(input, index);
                if (formatted_variable) {
                    result += *formatted_variable;
                }
                else {
                    break;
                }
            }

            //Escape character, fall through.
        }
        else if (input[index] == L'}') {

            ++index;

            //Invalid character.
            if (input[index] != L'}') {
                break;
            }

            //Escape character, fall through.
        }

        result += input[index];
        ++index;
    }

    if (index < input.size()) {
        result += input.substr(index);
    }

    return result;
}


std::optional<std::wstring> VariableFormatter::FormatVariable(
    std::wstring_view input, 
    std::size_t& index) {

    if (index >= input.size()) {
        return std::nullopt;
    }

    std::size_t current_index = index;
    for ( ; current_index < input.size(); ++current_index) {

        if (input[current_index] == L'}') {
            break;
        }
    }
}

}