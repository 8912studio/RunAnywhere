#include "main/toolbar/toolbar_button.h"
#include <zaf/graphic/image/image.h>

namespace ra {

ZAF_OBJECT_IMPL(ToolbarButton);

void ToolbarButton::Initialize() {

    __super::Initialize();

    SetFixedSize(zaf::Size{ 14, 14 });
    SetCanFocused(false);

    SetBorder({});
    SetBackgroundImageLayout(zaf::ImageLayout::Stretch);
}


void ToolbarButton::UpdateStyle() {

    __super::UpdateStyle();
    SetBackgroundColor(zaf::Color::Transparent());
}


void ToolbarButton::SetImageName(const std::wstring& name) {

    SetBackgroundImagePicker([this, name](const zaf::Control& control) {
    
        std::wstring uri{ L"res:///resource/" };
        uri += name;
        if (control.ContainsMouse()) {
            uri += L"_highlight";
        }
        uri += L".png";
        return zaf::Image::FromURI(uri, control.GetDPI());
    });
}

}