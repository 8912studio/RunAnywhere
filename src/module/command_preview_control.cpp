#include "module/command_preview_control.h"
#include <zaf/base/as.h>

namespace ra::mod {

void CommandPreviewControl::SetStyle(CommandDisplayStyle style) {
    style_ = style;
    OnStyleChanged();
}


void CommandPreviewControl::OnStyleChanged() {

}


zaf::Frame CommandPreviewControl::GetExpectedMargin() {
    return zaf::Frame{ 15, 0, 15, 0 };
}

}