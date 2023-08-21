#include "main/input/text_block_object.h"

namespace ra::main::input {

TextBlockObject::TextBlockObject(std::shared_ptr<TextBlockData> data) : 
    ArgumentObject(std::move(data)) {

}


TextBlockObject::TextBlockObject(std::wstring text) : 
    TextBlockObject(std::make_shared<TextBlockData>(std::move(text))) {

}


GUID TextBlockObject::ClassID() const {
    return { 0xb472b6af, 0xe996, 0x4120, { 0xab, 0xc8, 0x97, 0xa8, 0xf8, 0x65, 0x3d, 0x3b } };
}


std::shared_ptr<ArgumentData> TextBlockObject::CreateData(std::wstring text) {
    return std::make_shared<TextBlockData>(std::move(text));
}

}