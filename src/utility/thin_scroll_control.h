#pragma once

#include <zaf/control/scrollable_control.h>

namespace ra::utility {

class ThinScrollControl : public zaf::ScrollableControl {
public:
    ZAF_DECLARE_TYPE;

public:
    void AfterParse() override;
};

}