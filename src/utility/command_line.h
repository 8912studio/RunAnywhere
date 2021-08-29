#pragma once

#include <string>
#include <vector>

namespace ra::utility {

class CommandLine {
public:
    explicit CommandLine(const std::wstring& text);

    CommandLine(const CommandLine&) = delete;
    CommandLine& operator=(const CommandLine&) = delete;

    const std::wstring& Text() const {
        return text_;
    }

    const std::vector<std::wstring>& AllParts() const {
        return all_parts_;
    }

    const std::wstring& Command() const {
        return command_;
    }

    const std::vector<std::wstring>& Arguments() const {
        return arguments_;
    }

private:
    std::wstring text_;
    std::vector<std::wstring> all_parts_;
    std::wstring command_;
    std::vector<std::wstring> arguments_;
};

}