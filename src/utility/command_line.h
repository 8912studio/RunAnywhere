#pragma once

#include <functional>
#include <string_view>
#include <vector>
#include <zaf/base/non_copyable.h>

namespace ra::utility {

class CommandLine : zaf::NonCopyable {
public:
    using ObjectTextGetter = std::function<std::wstring(int object_index)>;

public:
    explicit CommandLine(
        std::wstring_view text, 
        const ObjectTextGetter& object_text_getter = nullptr);

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