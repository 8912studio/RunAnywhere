#include "environment_variable_manager.h"
#include <Windows.h>
#include <UserEnv.h>
#include <vector>
#include <zaf/base/handle.h>

namespace ra {

EnvironmentVariableManager& EnvironmentVariableManager::Instance() {
    static EnvironmentVariableManager instance;
    return instance;
}


void EnvironmentVariableManager::Initialize() {

    last_variables_ = LoadCurrentVariables();
}


void EnvironmentVariableManager::Update() {

    auto current_variables = LoadCurrentVariables();

    std::map<std::wstring, std::wstring> new_variables;
    std::map<std::wstring, std::wstring> modified_variables;
    std::vector<std::wstring> deleted_variables;
    DiffVariables(
        current_variables, 
        last_variables_, 
        new_variables, 
        modified_variables, 
        deleted_variables);

    for (const auto& each_pair : new_variables) {
        SetEnvironmentVariable(each_pair.first.c_str(), each_pair.second.c_str());
    }

    for (const auto& each_pair : modified_variables) {
        SetEnvironmentVariable(each_pair.first.c_str(), each_pair.second.c_str());
    }

    for (const auto& each_name : deleted_variables) {
        SetEnvironmentVariable(each_name.c_str(), L"");
    }

    last_variables_ = std::move(current_variables);
}


std::map<std::wstring, std::wstring> EnvironmentVariableManager::LoadCurrentVariables() {

    std::map<std::wstring, std::wstring> result;

    zaf::Handle token_handle;
    BOOL is_succeeded = OpenProcessToken(GetCurrentProcess(), TOKEN_READ, token_handle.Pointer());
    if (!is_succeeded) {
        return result;
    }

    wchar_t* environment_strings{};
    is_succeeded = CreateEnvironmentBlock(
        reinterpret_cast<LPVOID*>(&environment_strings),
        token_handle.Value(),
        FALSE);

    if (is_succeeded) {

        result = ParseEnvironmentStrings(environment_strings);
        DestroyEnvironmentBlock(environment_strings);
    }

    return result;
}


std::map<std::wstring, std::wstring> EnvironmentVariableManager::ParseEnvironmentStrings(
    const wchar_t* strings) {

    std::map<std::wstring, std::wstring> result;

    const wchar_t* name_begin = strings;
    const wchar_t* value_begin = strings;
    const wchar_t* current = strings;
    while (true) {

        if (*current == 0) {

            if (current == name_begin) {
                break;
            }

            if (value_begin > name_begin) {
                std::wstring name(name_begin, value_begin - 1 - name_begin);
                std::wstring value(value_begin, current - value_begin);
                result[std::move(name)] = std::move(value);
            }

            name_begin = current + 1;
            value_begin = name_begin;
            ++current;
        }
        else if (*current == L'=') {

            if (value_begin == name_begin) {
                value_begin = current + 1;
            }
            ++current;
        }
        else {
            ++current;
        }
    }

    return result;
}


void EnvironmentVariableManager::DiffVariables(
    const std::map<std::wstring, std::wstring>& current,
    const std::map<std::wstring, std::wstring>& last,
    std::map<std::wstring, std::wstring>& new_variables,
    std::map<std::wstring, std::wstring>& modified_variables,
    std::vector<std::wstring>& deleted_variables) {

    auto current_iterator = current.begin();
    auto last_iterator = last.begin();

    while (current_iterator != current.end() && 
           last_iterator != last.end()) {

        if (current_iterator->first == last_iterator->first) {

            if (current_iterator->second != last_iterator->second) {
                modified_variables.insert(*current_iterator);
            }

            ++current_iterator;
            ++last_iterator;
        }
        else if (current_iterator->first > last_iterator->first) {

            deleted_variables.push_back(last_iterator->first);
            ++last_iterator;
        }
        else if (current_iterator->first < last_iterator->first) {

            new_variables.insert(*current_iterator);
            ++current_iterator;
        }
    }

    for (; current_iterator != current.end(); ++current_iterator) {
        new_variables.insert(*current_iterator);
    }

    for (; last_iterator != last.end(); ++last_iterator) {
        deleted_variables.push_back(last_iterator->first);
    }
}

}