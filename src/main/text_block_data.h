#pragma once

#include <zaf/object/object.h>

namespace ra {

class TextBlockData : public zaf::Object {
public:
    explicit TextBlockData(std::wstring text) : text_(std::move(text)) {

    }

    const std::wstring& Text() const {
        return text_;
    }

private:
    std::wstring text_;
};

}