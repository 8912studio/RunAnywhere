#pragma once

#include <zaf/object/object.h>

namespace ra::main::input {

class ArgumentData : public zaf::Object {
public:
    explicit ArgumentData(std::wstring text) : text_(std::move(text)) {

    }

    const std::wstring& Text() const {
        return text_;
    }

private:
    std::wstring text_;
};

}