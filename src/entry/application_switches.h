#pragma once

#include <optional>
#include <string>
#include <vector>

namespace ra::entry {

class ApplicationSwithes {
public:
    explicit ApplicationSwithes(const std::wstring& command_line_text);

    bool HasSwitch(const std::wstring& name) const;
    std::optional<std::wstring> GetSwitchValue(const std::wstring& name) const;

    std::wstring ToString() const;

private:
    std::vector<std::pair<std::wstring, std::wstring>> pairs_;
};


constexpr const wchar_t* RegisterSwitchName = L"register";
constexpr const wchar_t* ImportSwitchName = L"import";

}