#include "module/common/text_preview_control.h"
#include <zaf/object/type_definition.h>

namespace ra::mod {

ZAF_DEFINE_TYPE(TextPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/text_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void TextPreviewControl::SetDisplayMode(TextDisplayMode mode) {
    textContent->SetDisplayMode(mode);
}


void TextPreviewControl::SetText(std::wstring text) {

    if (text.empty()) {
        textContent->SetIsVisible(false);
        emptyLabel->SetIsVisible(true);
    }
    else {
        textContent->SetIsVisible(true);
        textContent->SetText(std::move(text));
        emptyLabel->SetIsVisible(false);
    }
}


zaf::Frame TextPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    //Use thickness of scroll bars as margins.
    result.right = 0;
    result.bottom = 0;
    return result;
}

}