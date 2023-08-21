#pragma once

#include "main/input/argument_data.h"

namespace ra::main::input {

class ActivePathData : public ArgumentData {
public:
    explicit ActivePathData(std::wstring text) : ArgumentData(std::move(text)) {

    }

    std::wstring ToPlainText() const override {
        return L"@=" + Text();
    }
};

}