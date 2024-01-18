#pragma once

#include <zaf/control/scroll_box.h>

namespace ra::utility {

class ThinScrollControl : public zaf::ScrollBox {
public:
    ZAF_DECLARE_TYPE;

public:
    void AfterParse() override;
};

}