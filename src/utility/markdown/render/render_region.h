#pragma once

#include <zaf/control/control.h>

namespace ra::utility::markdown::render {

class RenderRegion : public zaf::Control {
public:
    virtual void ChangeSelection(
        const zaf::Point& current_position,
        const zaf::Point& begin_position) = 0;
};

}