#pragma once

#include <zaf/base/event/event.h>
#include <zaf/control/control.h>
#include "main/command_display_style.h"

namespace ra::mod {

class CommandPreviewControl;

class CommandPreviewContentChangedInfo : public zaf::EventInfo {
public:
    explicit CommandPreviewContentChangedInfo(
        const std::shared_ptr<CommandPreviewControl>& preview_control);
};


class CommandPreviewControl : public zaf::Control {
public:
    CommandDisplayStyle Style() const {
        return style_;
    }

    void SetStyle(CommandDisplayStyle style);

    zaf::Observable<CommandPreviewContentChangedInfo> ContentChangedEvent() const {
        return content_changed_event_.GetObservable();
    }

    virtual zaf::Frame GetExpectedMargin();

protected:
    virtual void OnStyleChanged();

    void RaiseContentChangedEvent();

private:
    CommandDisplayStyle style_{ CommandDisplayStyle::Normal };
    zaf::Event<CommandPreviewContentChangedInfo> content_changed_event_;
};

}