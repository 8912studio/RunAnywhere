#include "module/extension/extension_command_preview_control.h"
#include <zaf/base/string/trim.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/object/type_definition.h>

namespace ra::mod::extension {

ZAF_DEFINE_TYPE(ExtensionCommandPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/extension/extension_command_preview_control.xaml")
ZAF_DEFINE_TYPE_END


void ExtensionCommandPreviewControl::AfterParse() {

    __super::AfterParse();

    scrollControl->VerticalScrollBar()->SetSmallChange(14);

    Subscriptions() += scrollControl->RectChangedEvent().Subscribe(
        [this](const zaf::RectChangedInfo& event_info) {
    
            if (scrollControl->Width() != event_info.PreviousRect().size.width) {
                ResetTextBoxHeight();
            }
        }
    );
}


void ExtensionCommandPreviewControl::SetText(const std::wstring& text) {

    //A known bug of TextBox: if there is a line break in the end of text, vertical scroll bar can
    //not reach the bottom. So do a trimming first. 
    textBox->SetText(zaf::ToTrimmed(text));
    ResetTextBoxHeight();
}


void ExtensionCommandPreviewControl::ResetTextBoxHeight() {

    zaf::Size max_size{ 
        scrollControl->Width() - scrollControl->VerticalScrollBarThickness(),
        std::numeric_limits<float>::max() 
    };

    auto preferred_size = textBox->CalculatePreferredSize(max_size);

    const float min_height = 40.f;
    const float max_height = 500.f;
    
    float height = std::max(preferred_size.height, min_height);
    height = std::min(height, max_height);
    scrollControl->SetFixedHeight(height);
}

}