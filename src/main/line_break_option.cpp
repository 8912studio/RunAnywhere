#include "main/line_break_option.h"
#include <zaf/object/type_definition.h>

namespace ra {

ZAF_DEFINE_TYPE(LineBreakOption)
ZAF_DEFINE_TYPE_END;

void LineBreakOption::Initialize() {

    __super::Initialize();

    this->SetFixedSize(zaf::Size{ 32, 18 });
    this->SetFontSize(10);
    this->SetTextAlignment(zaf::TextAlignment::Center);
    this->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    this->SetBorder(zaf::Frame{ 1 });
    UpdateAppearance();
}


void LineBreakOption::OnIsSelectedChanged() {

    __super::OnIsSelectedChanged();

    UpdateAppearance();
}


void LineBreakOption::UpdateAppearance() {

    zaf::Color color = zaf::Color::FromRGB(IsSelected() ? 0x008833 : 0xbbbbbb);
    this->SetBorderColor(color);
    this->SetTextColor(color);

    this->SetFontWeight(IsSelected() ? zaf::FontWeight::Bold : zaf::FontWeight::Regular);
}

}