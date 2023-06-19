#include "module/common/error_view.h"
#include <zaf/creation.h>
#include <zaf/object/type_definition.h>

namespace ra::mod {

ZAF_DEFINE_TYPE(ErrorView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/error_view.xaml")
ZAF_DEFINE_TYPE_END;

void ErrorView::AfterParse() {

    __super::AfterParse();
}


void ErrorView::SetErrorText(const std::wstring& text) {
    errorText->SetText(text);
}

}