#pragma once

#include <string_view>
#include <zaf/base/non_copyable.h>

namespace ra::utility::markdown::render {

class SelectedTextBuilder : zaf::NonCopyableNonMovable {
public:
    void Append(std::wstring_view text) {
        text_.append(text);
    }

    void AppendLineBreak() {
        text_.append(L"\r\n");
    }

    const std::wstring& Text() const {
        return text_;
    }

private:
    std::wstring text_;
};

}