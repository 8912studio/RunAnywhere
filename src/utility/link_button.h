#pragma once

#include <zaf/control/button.h>

namespace ra::utility {

class LinkButton : public zaf::Button {
public:
    ZAF_OBJECT;

protected:
    void Initialize() override;
    void UpdateStyle() override;
};

ZAF_OBJECT_BEGIN(LinkButton)
ZAF_OBJECT_END

}