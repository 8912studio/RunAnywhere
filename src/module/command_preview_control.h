#pragma once

#include <zaf/control/control.h>

namespace ra::mod {

class CommandPreviewControl : public zaf::Control {
public:
    virtual zaf::Frame GetExpectedMargin() {
        return zaf::Frame{ 15, 0, 15, 2 };
    }
};

}