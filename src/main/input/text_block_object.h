#pragma once

#include "main/input/argument_object.h"
#include "main/input/text_block_data.h"

namespace ra::main::input {

class TextBlockObject : public ArgumentObject {
public:
    explicit TextBlockObject(std::shared_ptr<TextBlockData> data);
    explicit TextBlockObject(std::wstring text);

    GUID ClassID() const override;
    std::shared_ptr<ArgumentData> CreateData(std::wstring text) override;
};

}