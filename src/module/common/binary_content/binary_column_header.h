#pragma once

#include <zaf/control/control.h>

namespace ra::mod {

class BinaryColumnHeader : public zaf::Control {
public:
    ZAF_OBJECT;

protected:
    void Initialize() override;

    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) const override;
};

ZAF_OBJECT_BEGIN(BinaryColumnHeader)
ZAF_OBJECT_END;

}