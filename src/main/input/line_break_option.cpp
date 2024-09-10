#include "main/input/line_break_option.h"

namespace ra::main::input {

ZAF_OBJECT_IMPL(LineBreakOption)

void LineBreakOption::Initialize() {

    __super::Initialize();

    this->SetFixedSize(zaf::Size{ 32, 18 });
    this->SetFontSize(11);
    this->SetTextAlignment(zaf::dwrite::TextAlignment::Center);
    this->SetParagraphAlignment(zaf::dwrite::ParagraphAlignment::Center);
    this->SetBorder(zaf::Frame{ 1 });
    UpdateAppearance();
}


void LineBreakOption::SetCheckState(zaf::CheckState state) {
    check_state_ = state;
    UpdateAppearance();
}


void LineBreakOption::UpdateAppearance() {

    auto update_guard = this->BeginUpdate();

    zaf::Color color = zaf::Color::FromRGB(
        check_state_ == zaf::CheckState::Checked ? 0x009933 : 
        check_state_ == zaf::CheckState::Indeterminate ? 0x999999 :
        0xbbbbbb);

    this->SetBorderColor(color);
    this->SetTextColor(color);

    this->SetFontWeight(
        check_state_ != zaf::CheckState::Unchecked ? 
        zaf::FontWeight::Bold : 
        zaf::FontWeight::Regular);
}

}