#pragma once

#include <string>
#include <zaf/base/range.h>
#include "help/markdown/render/text_style.h"

namespace ra::help::markdown::render {

class RangedTextStyle {
public:
    zaf::Range range;
    TextStyle style;
};

class StyledText {
public:
    const std::wstring& Text() const {
        return text_;
    }

    const std::vector<RangedTextStyle>& Styles() const {
        return styles_;
    }

    void Append(std::wstring_view text) {
        text_ += text;
    }

    bool HasPendingText() const {

        if (text_.empty()) {
            return false;
        }

        if (styles_.empty()) {
            return true;
        }

        return styles_.back().range.EndIndex() < text_.length();
    }

    void AddStyleToPendingText(TextStyle style) {

        if (!HasPendingText()) {
            return;
        }

        zaf::Range range;
        if (styles_.empty()) {
            range.index = 0;
            range.length = text_.length();
        }
        else {
            range.index = styles_.back().range.EndIndex();
            range.length = text_.length() - range.index;
        }

        styles_.push_back(RangedTextStyle{ 
            range, 
            std::move(style) 
        });
    }

    bool CheckConstraint() const {

        if (text_.empty()) {
            return styles_.empty();
        }

        if (styles_.empty()) {
            return false;
        }

        return styles_.back().range.EndIndex() == text_.length();
    }

private:
    std::wstring text_;
    std::vector<RangedTextStyle> styles_;
};

}