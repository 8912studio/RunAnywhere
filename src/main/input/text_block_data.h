#pragma once

#include "main/input/argument_data.h"

namespace ra::main::input {

class TextBlockData : public ArgumentData {
public:
    explicit TextBlockData(std::wstring text) : ArgumentData(std::move(text)) {

    }
};

}