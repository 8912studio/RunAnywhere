#pragma once

#include <string>
#include "help/content/line.h"

namespace ra::help::content {

class SingleTextLine : public Line {
public:
    SingleTextLine() = default;
    SingleTextLine(const std::wstring& text) : text_(text) { }

    const std::wstring& Text() const {
        return text_;
    }

private:
    std::wstring text_;
};

}