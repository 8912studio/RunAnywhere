#include "module/command_preview_control.h"
#include <zaf/base/as.h>

namespace ra::mod {

void CommandPreviewControl::SetStyle(CommandDisplayStyle style) {
    style_ = style;
    OnStyleChanged();
}


void CommandPreviewControl::OnStyleChanged() {

}


void CommandPreviewControl::RaiseContentChangedEvent() {

    content_changed_event_.Raise(CommandPreviewContentChangedInfo{
        zaf::As<CommandPreviewControl>(shared_from_this())
    });
}


zaf::Frame CommandPreviewControl::GetExpectedMargin() {
    return zaf::Frame{ 15, 0, 15, 0 };
}


CommandPreviewContentChangedInfo::CommandPreviewContentChangedInfo(
    const std::shared_ptr<CommandPreviewControl>& preview_control)
    :
    EventInfo(preview_control) {

}

}