#pragma once

#include <zaf/control/scroll_box.h>

namespace ra::utility {

class ThinScrollControl : public zaf::ScrollBox {
public:
    ZAF_OBJECT;

public:
    void AfterParse() override;
};

ZAF_OBJECT_BEGIN(ThinScrollControl)
ZAF_OBJECT_END;

}