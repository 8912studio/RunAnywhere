#include "module/command_preview_control.h"

namespace ra::mod {

void CommandPreviewControl::SetStyle(PreviewStyle style) {
    style_ = style;
    OnStyleChanged();
}


void CommandPreviewControl::OnStyleChanged() {

}


zaf::Frame CommandPreviewControl::GetExpectedMargin() {
    return zaf::Frame{ 15, 0, 15, 0 };
}

}