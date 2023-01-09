#pragma once

#include <zaf/window/window.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/control/text_box.h>

namespace ra {

class LicenseWindow : public zaf::Window {
public:
    ZAF_DECLARE_TYPE;

public:
    static void ShowInstance();

protected:
    void AfterParse() override;

private:
    LicenseWindow() = default;

    void InitializeScrollableControl();
    void InitializeTextBox();

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollableControl);
    ZAF_BIND_CONTROL(zaf::TextBox, licenseContentTextBox);
};

}