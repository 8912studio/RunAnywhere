#include "module/tool/text_transform/text_preview_control.h"
#include <zaf/object/type_definition.h>
#include "module/common/error_messages.h"

namespace ra::mod {

ZAF_DEFINE_TYPE(TextPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/text_transform/text_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void TextPreviewControl::SetText(std::wstring text) {

    if (text.empty()) {
        textContent->SetIsVisible(false);
        errorView->ShowHintText(ErrorMessages::NoContentToDisplay);
        errorView->SetIsVisible(true);
    }
    else {
        textContent->SetIsVisible(true);
        textContent->SetText(std::move(text));
        errorView->SetIsVisible(false);
    }
}


void TextPreviewControl::OnStyleChanged() {
    textContent->ChangeStyle(Style());
    errorView->ChangeStyle(Style());
}


zaf::Frame TextPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    //Use thickness of scroll bars as margins.
    result.right = 0;
    result.bottom = 0;
    return result;
}

}