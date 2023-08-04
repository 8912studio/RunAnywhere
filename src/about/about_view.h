#pragma once

#include <zaf/control/button.h>
#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>

namespace ra {

class AboutView : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

protected:
    void AfterParse() override;

private:
    void InitializeGithubButton();
    void InitializeLicenseButton();

private:
    ZAF_BIND_CONTROL(zaf::Label, versionLabel);
    ZAF_BIND_CONTROL(zaf::Button, githubButton);
    ZAF_BIND_CONTROL(zaf::Button, licenseButton);
};

}