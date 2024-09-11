#pragma once

#include "main/input/argument_object.h"
#include "main/input/text_block_data.h"

namespace ra::main::input {

class TextBlockObject : public ArgumentObject {
public:
    ZAF_OBJECT;

    explicit TextBlockObject(std::shared_ptr<TextBlockData> data);
    explicit TextBlockObject(std::wstring text);

    std::shared_ptr<ArgumentData> CreateData(std::wstring text) override;
    zaf::Color GetBackgroundColor() const override;
    std::shared_ptr<ArgumentObjectWindow> CreateArgumentObjectWindow() override;

    std::shared_ptr<zaf::textual::InlineObject> Clone() const override;
};

ZAF_OBJECT_BEGIN(TextBlockObject);
ZAF_OBJECT_END;

}