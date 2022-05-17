#pragma once

#include <map>
#include <string>
#include <vector>

namespace ra {

class EnvironmentVariableManager {
public:
    static EnvironmentVariableManager& Instance();

    //For unittest.
    static std::map<std::wstring, std::wstring> ParseEnvironmentStrings(const wchar_t* strings);
    static void DiffVariables(
        const std::map<std::wstring, std::wstring>& current,
        const std::map<std::wstring, std::wstring>& last,
        std::map<std::wstring, std::wstring>& new_variables, 
        std::map<std::wstring, std::wstring>& modified_variables,
        std::vector<std::wstring>& deleted_variables);

public:
    EnvironmentVariableManager(const EnvironmentVariableManager&) = delete;
    EnvironmentVariableManager& operator=(const EnvironmentVariableManager&) = delete;

    void Initialize();
    void Update();

private:
    static std::map<std::wstring, std::wstring> LoadCurrentVariables();

private:
    EnvironmentVariableManager() = default;

private:
    std::map<std::wstring, std::wstring> last_variables_;
};

}