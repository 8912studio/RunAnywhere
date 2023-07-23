#pragma once

#include <zaf/control/control.h>

namespace ra::mod {

enum class PreviewStyle {
    Normal,
    Historical,
};

class CommandPreviewControl : public zaf::Control {
public:
    PreviewStyle Style() const {
        return style_;
    }

    void SetStyle(PreviewStyle style);

    virtual zaf::Frame GetExpectedMargin();

protected:
    virtual void OnStyleChanged();

private:
    PreviewStyle style_{ PreviewStyle::Normal };
};

}