#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>

namespace ra::mod {

class ErrorView : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

    void ShowErrorText(const std::wstring& text);
    void ShowHintText(const std::wstring& text);

protected:
    void AfterParse() override;

private:
    void ShowText(const std::wstring& text, bool is_error);

private:
    ZAF_BIND_CONTROL(zaf::Control, errorIcon);
    ZAF_BIND_CONTROL(zaf::Label, textLabel);
};

}