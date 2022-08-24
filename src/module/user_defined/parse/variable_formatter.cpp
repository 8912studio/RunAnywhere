#include "module/user_defined/parse/variable_formatter.h"
#include <zaf/base/error/error.h>
#include <zaf/base/registry/registry.h>
#include <zaf/base/string/case_conversion.h>
#include "module/active_path/active_path_modifying.h"
#include "module/active_path/active_path_option_parsing.h"

namespace ra::module::user_defined {
namespace {

bool IsValidVariableNameChar(wchar_t ch) {
    return (ch == L'_') || std::isalnum(ch);
}


bool SplitVariableParts(
    std::wstring_view variable_inner,
    std::wstring_view& name,
    std::wstring_view& modifier) {

    std::size_t modifier_begin_index{};
    for (modifier_begin_index = 0; 
         modifier_begin_index < variable_inner.size(); 
         ++modifier_begin_index) {

        if (!IsValidVariableNameChar(variable_inner[modifier_begin_index])) {
            break;
        }
    }

    name = variable_inner.substr(0, modifier_begin_index);
    if (name.empty()) {
        return false;
    }

    modifier = variable_inner.substr(modifier_begin_index);
    return true;
}

}


VariableFormatter::VariableFormatter(
    const std::shared_ptr<BundleMeta>& bundle_meta,
    const context::ActivePath& active_path)
    :
    bundle_meta_(bundle_meta),
    active_path_(active_path) {

    ZAF_EXPECT(bundle_meta_);
}


std::wstring VariableFormatter::Format(std::wstring_view input) const {

    std::wstring result;

    std::size_t index{};
    while (index < input.size()) {

        if (input[index] == L'{') {

            ++index;

            if (index < input.size()) {

                //Escape character
                if (input[index] == L'{') {

                    result += L'{';
                    ++index;
                    continue;
                }

                //Variable
                auto formatted_variable = FormatVariable(input, index);
                if (formatted_variable) {
                    result += *formatted_variable;
                    continue;
                }
            }

            //Bad variable, interrupt.
            --index;
            break;
        }
        else if (input[index] == L'}') {

            ++index;

            if (index < input.size()) {

                //Escape character.
                if (input[index] == L'}') {

                    result += input[index];
                    ++index;
                    continue;
                }
            }

            //Invalid character, interrupt.
            --index;
            break;
        }
        else {

            result += input[index];
            ++index;
        }
    }

    if (index < input.size()) {
        result += input.substr(index);
    }

    return result;
}


std::optional<std::wstring> VariableFormatter::FormatVariable(
    std::wstring_view input, 
    std::size_t& index) const {

    if (index >= input.size()) {
        return std::nullopt;
    }

    std::size_t current_index = index;
    for ( ; current_index < input.size(); ++current_index) {
        if (input[current_index] == L'}') {
            break;
        }
    }

    if (current_index == input.size()) {
        return std::nullopt;
    }

    std::optional<std::wstring> result;
    auto variable_inner = input.substr(index, current_index - index);
    if (variable_inner.empty()) {
        return std::nullopt;
    }

    if (variable_inner.front() == L'@') {
        result = FormatActivePathVariable(variable_inner);
    }
    else {
        result = FormatGeneralVariable(variable_inner);
    }

    if (result) {
        index = current_index + 1; //+1 for pass '}'
    }

    return result;
}


std::optional<std::wstring> VariableFormatter::FormatActivePathVariable(
    std::wstring_view variable) const {

    auto active_path_option = active_path::ParseActivePathOption(variable.substr(1));

    auto modified_active_path = active_path::ModifyActivePathByOption(
        active_path_,
        active_path_option);

    return modified_active_path.GetPath().wstring();
}


std::optional<std::wstring> VariableFormatter::FormatGeneralVariable(
    std::wstring_view variable) const {

    std::wstring_view name;
    std::wstring_view modifier;
    if (!SplitVariableParts(variable, name, modifier)) {
        return std::nullopt;
    }

    auto variable_modifier = ParseVariableModifier(modifier);
    if (!variable_modifier) {
        return std::nullopt;
    }

    return GetVariableContent(name, *variable_modifier);
}


std::optional<VariableFormatter::VariableModifier> VariableFormatter::ParseVariableModifier(
    std::wstring_view modifier) {

    VariableModifier result;
    for (auto each_ch : modifier) {

        if (each_ch == L'!') {
            result.do_not_expand = true;
        }
        else if (each_ch == L'?') {
            result.do_not_check_existent = true;
        }
        else if (each_ch == L'.') {
            result.expand_to_directory = true;
        }
        else {
            return std::nullopt;
        }
    }
    return result;
}


std::wstring VariableFormatter::GetVariableContent(
    std::wstring_view name, 
    const VariableModifier& modifier) const {

    for (const auto& each_property : bundle_meta_->GlobalProperties()) {

        if (each_property.first == name) {

            auto content = ExpandVariableContent(each_property.second, modifier);
            if (content) {
                return *content;
            }
        }
    }

    return std::wstring{};
}


std::optional<std::wstring> VariableFormatter::ExpandVariableContent(
    const std::wstring& content,
    const VariableModifier& modifier) {

    if (modifier.do_not_expand) {
        return content;
    }

    std::wstring pending_content;

    auto registry_content = TryToExpandRegistryContent(content);
    if (registry_content) {
        pending_content = *registry_content;
    }
    else {
        pending_content = content;
    }

    if (modifier.do_not_check_existent) {
        return pending_content;
    }

    std::filesystem::path path{ pending_content };
    std::error_code error_code;
    if (!std::filesystem::exists(path, error_code)) {
        return std::nullopt;
    }

    if (modifier.expand_to_directory) {
        if (std::filesystem::is_regular_file(path, error_code)) {
            return path.parent_path();
        }
    }

    return path;
}


std::optional<std::wstring> VariableFormatter::TryToExpandRegistryContent(
    const std::wstring& content) {

    auto first_delimiter = content.find(L'\\');
    if (first_delimiter == std::wstring::npos) {
        return std::nullopt;
    }

    auto root_key_name = content.substr(0, first_delimiter);
    zaf::Lowercase(root_key_name);

    zaf::RegistryKey root_key;
    if (root_key_name == L"hkcu") {
        root_key = zaf::Registry::CurrentUser();
    }
    else if (root_key_name == L"hklm") {
        root_key = zaf::Registry::LocalMachine();
    }
    else {
        return std::nullopt;
    }

    auto last_delimiter = content.find_last_of(L'@');
    if (last_delimiter == std::wstring::npos) {
        return std::nullopt;
    }

    auto sub_key_path = content.substr(
        first_delimiter + 1,
        last_delimiter - first_delimiter - 1);

    auto value_name = content.substr(last_delimiter + 1);

    try {

        auto sub_key = root_key.OpenSubKey(sub_key_path);
        auto value = sub_key.GetValue(value_name);

        //Only supports string value.
        if (value.Type() == zaf::RegistryValueType::String) {
            return value.ToString();
        }

        return std::wstring{};
    }
    catch (const zaf::Error&) {
        return std::nullopt;
    }
}

}