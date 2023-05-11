#pragma once

#include <zaf/control/scroll_bar.h>

namespace ra::utility {

class ThinScrollBar : public zaf::ScrollBar {
public:
    ZAF_DECLARE_TYPE;

protected:
    void Initialize() override;
};

}