#pragma once

#include <zaf/control/check_state.h>
#include <zaf/control/textual_control.h>

namespace ra::main::input {

class LineBreakOption : public zaf::TextualControl {
public:
    ZAF_DECLARE_TYPE;

    void SetCheckState(zaf::CheckState state);

protected:
    void Initialize() override;

private:
    void UpdateAppearance();

private:
    zaf::CheckState check_state_{ zaf::CheckState::Unchecked };
};

}