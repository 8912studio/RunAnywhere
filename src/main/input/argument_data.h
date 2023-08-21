#pragma once

#include <zaf/object/object.h>

namespace ra::main::input {

class ArgumentData : public zaf::Object {
public:
    const std::wstring& Text() const {
        return text_;
    }

    virtual std::wstring ToPlainText() const {
        return text_;
    }

protected:
    explicit ArgumentData(std::wstring text) : text_(std::move(text)) {

    }

private:
    std::wstring text_;
};

}