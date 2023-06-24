#pragma once

#include <zaf/control/control.h>

namespace ra::mod {

class BinaryColumnHeader : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

protected:
    void Initialize() override;

    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) override;
};

}