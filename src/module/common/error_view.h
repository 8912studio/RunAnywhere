#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>

namespace ra::mod {

class ErrorView : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

    void SetErrorText(const std::wstring& text);

protected:
    void AfterParse() override;

private:
    ZAF_BIND_CONTROL(zaf::Label, errorText);
};

}