#include "main/input/text_block_object.h"
#include <zaf/creation.h>
#include "main/input/text_block_window.h"

namespace ra::main::input {

ZAF_OBJECT_IMPL(TextBlockObject);

TextBlockObject::TextBlockObject(std::shared_ptr<TextBlockData> data) : 
    ArgumentObject(std::move(data)) {

}


TextBlockObject::TextBlockObject(std::wstring text) : 
    TextBlockObject(std::make_shared<TextBlockData>(std::move(text))) {

}


std::shared_ptr<ArgumentData> TextBlockObject::CreateData(std::wstring text) {
    return std::make_shared<TextBlockData>(std::move(text));
}


zaf::Color TextBlockObject::GetBackgroundColor() const {
    return this->IsInSelectionRange() ? 
        zaf::Color::FromARGB(0x7d2EAFFF) : 
        zaf::Color::FromARGB(0x7d95D4FF);
}


std::shared_ptr<ArgumentObjectWindow> TextBlockObject::CreateArgumentObjectWindow() {
    return zaf::Create<TextBlockWindow>();
}

}