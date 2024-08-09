#pragma once

#include <zaf/window/window.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/scroll_box.h>
#include <zaf/control/rich_edit.h>

namespace ra {

class LicenseWindow : public zaf::Window {
public:
    ZAF_OBJECT;

public:
    static void ShowInstance();

protected:
    void AfterParse() override;

private:
    LicenseWindow() = default;

    void InitializeScrollableControl();
    void InitializeTextBox();

private:
    ZAF_BIND_CONTROL(zaf::ScrollBox, scrollableControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, licenseContentTextBox);
};

ZAF_OBJECT_BEGIN(LicenseWindow)
ZAF_OBJECT_RESOURCE_URI(L"res:///license_window.xaml")
ZAF_OBJECT_END

}