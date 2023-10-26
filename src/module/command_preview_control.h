#pragma once

#include <zaf/control/control.h>
#include "main/command_display_style.h"

namespace ra::mod {

class CommandPreviewControl : public zaf::Control {
public:
    CommandDisplayStyle Style() const {
        return style_;
    }

    void SetStyle(CommandDisplayStyle style);

    virtual zaf::Frame GetExpectedMargin();

protected:
    virtual void OnStyleChanged();

private:
    CommandDisplayStyle style_{ CommandDisplayStyle::Normal };
};

}