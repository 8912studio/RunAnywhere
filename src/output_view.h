#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>

class OutputView : public zaf::Control {
public:
    ZAF_DECLARE_REFLECTION_TYPE;

    void SetText(const std::wstring& text);

private:
    ZAF_BIND_CONTROL(zaf::Label, outputLabel);
};