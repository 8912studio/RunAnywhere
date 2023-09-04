#pragma once

#include <zaf/base/non_copyable.h>
#include "help/markdown/style_span.h"

namespace ra::help::markdown {

class StyledStringBuilder : zaf::NonCopyable {
public:
    StyledStringBuilder();

    void BeginNewStyle(StyleType style_type);
    void CommitStyle();

    void Append(wchar_t ch) {
        string_.append(1, ch);
    }

    void Append(std::wstring_view string) {
        string_.append(string);
    }

    void Append(std::wstring_view string, StyleSpan style_span);

    std::pair<std::wstring, StyleSpan> BuildIntermediate();

private:
    class NewStyleInfo {
    public:
        std::size_t absolute_index{};
        StyleSpan style_span;
    };

private:
    std::wstring string_;
    std::vector<NewStyleInfo> style_stack_;
};

}