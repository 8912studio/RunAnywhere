#pragma once

#include <string>

namespace ra::mod {

class CommandBrief {
public:
    CommandBrief(const std::wstring& command, const std::wstring& description) : 
        command_(command),
        description_(description) {

    }

    const std::wstring& Command() const {
        return command_;
    }

    const std::wstring Description() const {
        return description_;
    }

private:
    std::wstring command_;
    std::wstring description_;
};

}