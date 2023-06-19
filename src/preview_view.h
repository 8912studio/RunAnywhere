#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include "module/command_preview_control.h"

namespace ra {

class PreviewView : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

    void SetPreviewControl(const std::shared_ptr<mod::CommandPreviewControl>& control);
    void ClearPreviewControl();

    float GetExpectedHeight();

private:
    std::shared_ptr<mod::CommandPreviewControl> preview_control_;
};

}