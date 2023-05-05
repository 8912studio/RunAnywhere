#pragma once

#include <functional>
#include <string_view>
#include <vector>

namespace ra::utility {

class CommandLine {
public:
    using ObjectTextGetter = std::function<std::wstring(int object_index)>;

public:
    explicit CommandLine(
        std::wstring_view text, 
        const ObjectTextGetter& object_text_getter = nullptr);

    CommandLine(const CommandLine&) = delete;
    CommandLine& operator=(const CommandLine&) = delete;

    CommandLine(CommandLine&&) = default;
    CommandLine& operator=(CommandLine&&) = default;

    const std::wstring& RawText() const {
        return raw_text_;
    }

    const std::wstring& Command() const {
        return command_;
    }

    const std::vector<std::wstring>& Arguments() const {
        return arguments_;
    }

    std::vector<std::wstring> AllParts() const;

private:
    std::wstring raw_text_;
    std::wstring command_;
    std::vector<std::wstring> arguments_;
};

}