#include "main/input/active_path_window.h"

namespace ra::main::input {

ZAF_OBJECT_IMPL(ActivePathWindow);

void ActivePathWindow::AfterParse() {

    __super::AfterParse();

    Subscriptions() += edit->TextChangedEvent().Subscribe(std::bind([this]() {
    
        Relayout();
        RaiseTextChangedEvent();
    }));
}


void ActivePathWindow::SetIsReadOnly(bool is_read_only) {
    edit->SetIsEditable(!is_read_only);
}


std::wstring ActivePathWindow::GetText() {
    return edit->Text();
}


void ActivePathWindow::SetText(const std::wstring& text) {

    edit->SetText(text);
    Relayout();
}


zaf::Size ActivePathWindow::CalculateWindowContentSize() {

    constexpr float ContentHeight = 60;

    auto edit_size = edit->CalculatePreferredSize();
    float total_width = 
        edit_size.width +
        scrollContainer->Margin().Width() +
        label->Width() + label->Margin().Width() +
        container->Padding().Width();

    return zaf::Size{
        std::min(total_width, MaxWindowContentWidth),
        ContentHeight
    };
}


void ActivePathWindow::OnShow(const zaf::ShowInfo& event_info) {

    __super::OnShow(event_info);

    edit->SetIsFocused(true);
}

}