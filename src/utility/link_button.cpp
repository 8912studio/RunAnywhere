#include "utility/link_button.h"
#include <zaf/graphic/font/font.h>

namespace ra::utility {

ZAF_OBJECT_IMPL(LinkButton);

void LinkButton::Initialize() {

    __super::Initialize();

    this->SetBorder(zaf::Frame{});
    this->SetCanFocused(false);
    this->SetTextAlignment(zaf::TextAlignment::Center);
    this->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    this->SetAutoWidth(true);
    this->SetBackgroundColor(zaf::Color::Transparent());

    this->SetTextColorPicker(zaf::ColorPicker([](const Control& control) {
        if (control.IsMouseOver()) {
            return zaf::Color::FromRGB(0x2080ff);
        }
        else {
            return zaf::Color::FromRGB(0x2020ff);
        }
    }));

    zaf::Font font;
    font.size = 12;
    font.has_underline = true;
    this->SetFont(font);
}

}