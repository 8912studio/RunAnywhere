#pragma once

#include <string>
#include "help/content/line.h"

namespace ra::help::content {

class TwoPartsLine : public Line {
public:
    TwoPartsLine(const std::wstring& primary_text, const std::wstring& secondly_text) : 
        primary_text_(primary_text), 
        secondly_text_(secondly_text) { }

    const std::wstring& FirstText() const {
        return primary_text_;
    }

    const std::wstring& SecondText() const {
        return secondly_text_;
    }

private:
    std::wstring primary_text_;
    std::wstring secondly_text_;
};

}