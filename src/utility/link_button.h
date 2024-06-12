#pragma once

#include <zaf/control/button.h>

namespace ra::utility {

class LinkButton : public zaf::Button {
public:
    ZAF_DECLARE_TYPE;

protected:
    void Initialize() override;
    void UpdateVisualState() override;
};

}