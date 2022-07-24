#include "entry/application_switches.h"
#include <optional>
#include <zaf/base/string/case_conversion.h>
#include "utility/command_line.h"

namespace ra::entry {

ApplicationSwithes::ApplicationSwithes(const std::wstring& command_line_text) {

    utility::CommandLine command_line(command_line_text);
    const auto& parts = command_line.AllParts();

    std::optional<std::pair<std::wstring, std::wstring>> current_switch;

    for (std::size_t index = 0; index < parts.size(); ++index) {

        const auto& current_part = parts[index];
        if (current_part.empty()) {
            continue;
        }

        if (current_part.front() == L'/') {

            //Finish current switch
            if (current_switch) {
                pairs_.push_back(*current_switch);
            }

            auto name = current_part.substr(1);
            if (!name.empty()) {

                //Begin a new switch
                current_switch = std::pair<std::wstring, std::wstring>{};
                current_switch->first = zaf::ToLowercased(name);
            }
            else {

                //Invalid switch name
                current_switch.reset();
            }
        }
        else {

            if (current_switch) {
                current_switch->second = current_part;
            }
        }
    }

    if (current_switch) {
        pairs_.push_back(*current_switch);
    }
}


bool ApplicationSwithes::HasSwitch(const std::wstring& name) const {

    auto lowered_name = zaf::ToLowercased(name);
    for (const auto& each_pair : pairs_) {

        if (each_pair.first == lowered_name) {
            return true;
        }
    }

    return false;
}


std::optional<std::wstring> ApplicationSwithes::GetSwitchValue(const std::wstring& name) const {

    auto lowered_name = zaf::ToLowercased(name);
    for (const auto& each_pair : pairs_) {

        if (each_pair.first == lowered_name) {
            return each_pair.second;
        }
    }

    return std::nullopt;
}


std::wstring ApplicationSwithes::ToString() const {

    std::wstring result;
    for (const auto& each_pair : pairs_) {

        result += L'/';
        result += each_pair.first;
        result += L' ';
        result += each_pair.second;
        result += L' ';
    }
    return result;
}

}