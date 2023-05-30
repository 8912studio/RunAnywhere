#pragma once

#include <zaf/control/textual_control.h>

namespace ra {

class LineBreakOption : public zaf::TextualControl {
public:
    ZAF_DECLARE_TYPE;

protected:
    void Initialize() override;
    void OnIsSelectedChanged() override;

private:
    void UpdateAppearance();
};

}