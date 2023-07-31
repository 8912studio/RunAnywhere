#include "main/toolbar/toolbar_button.h"
#include <zaf/graphic/image/image.h>
#include <zaf/object/type_definition.h>

namespace ra {

ZAF_DEFINE_TYPE(ToolbarButton)
ZAF_DEFINE_TYPE_PROPERTY(ImageName)
ZAF_DEFINE_TYPE_END;

void ToolbarButton::Initialize() {

    __super::Initialize();

    SetFixedSize(zaf::Size{ 14, 14 });
    SetCanFocused(false);

    SetBorder({});
    SetBackgroundColor(zaf::Color::Transparent());
    SetBackgroundImageLayout(zaf::ImageLayout::Stretch);
}


void ToolbarButton::SetImageName(const std::wstring& name) {

    SetBackgroundImagePicker([this, name](const zaf::Control& control) {
    
        std::wstring uri{ L"res:///resource/" };
        uri += name;
        if (control.ContainMouse()) {
            uri += L"_highlight";
        }
        uri += L".png";
        return zaf::Image::FromURI(uri, control.GetDPI());
    });
}

}