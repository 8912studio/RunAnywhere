#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/window/window.h>
#include "help/content/content.h"
#include "help/help_content_control.h"

namespace ra::help {

class HelpWindow : public zaf::Window {
public:
    ZAF_DECLARE_TYPE;

    void SetContent(const content::Content& content);

    void ScrollLine(bool scroll_up);
    void ScrollPage(bool scroll_up);

protected:
    void AfterParse() override;

private:
    void InitializeScrollControls();
    void InitializeScrollButtonContainer();
    void InitializeScrollButtons();
    void LayoutScrollButtonContainer();
    void OnNeedUpdateHeight(float new_height);

private:
    ZAF_BIND_CONTROL(zaf::Control, scrollButtonContainer);
    ZAF_BIND_CONTROL(zaf::Control, lineDownButton);
    ZAF_BIND_CONTROL(zaf::Control, lineUpButton);
    ZAF_BIND_CONTROL(zaf::Control, pageDownButton);
    ZAF_BIND_CONTROL(zaf::Control, pageUpButton);

    std::shared_ptr<HelpContentControl> content_control_;
};

}