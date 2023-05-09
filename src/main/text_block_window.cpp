#include "main/text_block_window.h"
#include <zaf/object/type_definition.h>

namespace ra {

ZAF_DEFINE_TYPE(TextBlockWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main/text_block_window.xaml")
ZAF_DEFINE_TYPE_END;

TextBlockWindow::TextBlockWindow(const zaf::COMObject<TextBlockObject>& object) : object_(object) {

    ZAF_EXPECT(object_);
}


void TextBlockWindow::AfterParse() {

    __super::AfterParse();

    textEdit->SetText(object_->Text());
}

}