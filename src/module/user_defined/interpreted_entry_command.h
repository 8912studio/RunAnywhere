#pragma once

#include <string>
#include <vector>

namespace ra::module::user_defined {

class InterpretedEntryCommand {
public:
    InterpretedEntryCommand(const std::wstring& base, const std::vector<std::wstring>& arguments) :
        base_(base),
        arguments_(arguments) {

    }

    const std::wstring& Base() const {
        return base_;
    }

    const std::vector<std::wstring>& Arguments() const {
        return arguments_;
    }

private:
    std::wstring base_;
    std::vector<std::wstring> arguments_;
};

}