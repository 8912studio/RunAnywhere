#pragma once

#include <zaf/control/scroll_bar.h>

namespace ra::utility {

class ThinScrollBar : public zaf::ScrollBar {
public:
    ZAF_OBJECT;

protected:
    void Initialize() override;
};

ZAF_OBJECT_BEGIN(ThinScrollBar)
ZAF_OBJECT_END;

}