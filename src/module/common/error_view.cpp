#include "module/common/error_view.h"
#include <zaf/creation.h>
#include <zaf/graphic/color.h>
#include <zaf/object/type_definition.h>

namespace ra::mod {

ZAF_DEFINE_TYPE(ErrorView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/error_view.xaml")
ZAF_DEFINE_TYPE_END;

void ErrorView::AfterParse() {

    __super::AfterParse();
}


void ErrorView::ShowErrorText(const std::wstring& text) {
    ShowText(text, true);
}


void ErrorView::ShowHintText(const std::wstring& text) {
    ShowText(text, false);
}


void ErrorView::ShowText(const std::wstring& text, bool is_error) {

    auto update_guard = this->BeginUpdate();
    errorIcon->SetIsVisible(is_error);
    textLabel->SetText(text);
    textLabel->SetTextColor(is_error ? zaf::Color::Black() : zaf::Color::FromRGB(0xAAAAAA));
}

}