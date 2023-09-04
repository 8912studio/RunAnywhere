#pragma once

#include <string>
#include <zaf/base/error/check.h>
#include "help/markdown/style_span.h"

namespace ra::help::markdown {

class StyledString {
public:
    StyledString(std::wstring string, StyleSpan style_span) : 
        string_(std::move(string)), 
        root_style_span_(std::move(style_span)) {

        ZAF_EXPECT(
            root_style_span_.range.index == 0 && 
            root_style_span_.range.length == string_.length());
    }

    const std::wstring& String() const {
        return string_;
    }

    const StyleSpan& RootStyleSpan() const {
        return root_style_span_;
    }

    friend bool operator==(const StyledString&, const StyledString&) = default;

private:
    std::wstring string_;
    StyleSpan root_style_span_;
};

}