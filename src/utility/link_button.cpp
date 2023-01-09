#include "utility/link_button.h"
#include <zaf/graphic/font/font.h>
#include <zaf/object/type_definition.h>

namespace ra::utility {

ZAF_DEFINE_TYPE(LinkButton)
ZAF_DEFINE_TYPE_END


void LinkButton::Initialize() {

    __super::Initialize();

    this->SetBackgroundColor(zaf::Color::Transparent());
    this->SetBorder(zaf::Frame{});
    this->SetCanFocused(false);
    this->SetTextAlignment(zaf::TextAlignment::Center);
    this->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    this->SetAutoWidth(true);

    zaf::Font font;
    font.size = 12;
    font.has_underline = true;
    this->SetFont(font);

    this->SetTextColorPicker([](const zaf::Control& control) {

        if (control.IsMouseOver()) {
            return zaf::Color::FromRGB(0x2080ff);
        }
        else {
            return zaf::Color::FromRGB(0x2020ff);
        }
    });
}

}