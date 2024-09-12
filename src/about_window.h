#pragma once

#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/window/dialog.h>

namespace ra {

class AboutWindow : public zaf::Dialog {
public:
    ZAF_OBJECT;

    static void ShowInstance();

protected:
    void AfterParse() override;

private:
    void InitializeVersionText();
    void InitializeGithubButton();
    void InitializeLicenseButton();
    void InitializeCopyrightText();

private:
    ZAF_BIND_CONTROL(zaf::Label, copyrightLabel);
    ZAF_BIND_CONTROL(zaf::Label, versionLabel);
    ZAF_BIND_CONTROL(zaf::Button, githubButton);
    ZAF_BIND_CONTROL(zaf::Button, licenseButton);
};

ZAF_OBJECT_BEGIN(AboutWindow)
ZAF_OBJECT_RESOURCE_URI(L"res:///about_window.xaml")
ZAF_OBJECT_END

}