#pragma once

#include <zaf/control/control.h>

namespace ra::mod {

enum class CommandDisplayStyle {
    Normal,
    Preserved,
};

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