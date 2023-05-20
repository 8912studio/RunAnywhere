#include "module/tool/text_transform/text_preview_control.h"
#include <zaf/object/type_definition.h>

namespace ra::module::tool::text_transform{

ZAF_DEFINE_TYPE(TextPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/text_transform/text_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void TextPreviewControl::AfterParse() {

    __super::AfterParse();

    scrollControl->SetAllowHorizontalScroll(false);
    scrollControl->SetAllowVerticalScroll(false);
}


void TextPreviewControl::SetText(const std::wstring& text) {

    richEdit->SetText(text);

    scrollControl->SetAllowHorizontalScroll(true);
    scrollControl->SetAllowVerticalScroll(true);
}

}