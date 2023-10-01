#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/window/window.h>
#include "help/markdown/element/element.h"
#include "utility/thin_border_window.h"

namespace ra::help {

class HelpWindow : public utility::ThinBorderWindow {
public:
    ZAF_DECLARE_TYPE;

    void SetContent(const markdown::element::Element& content);

    void ScrollLine(bool scroll_up);
    void ScrollPage(bool scroll_up);

protected:
    void AfterParse() override;

private:
    void InitializeScrollControls();
    void InitializeScrollButtonContainer();
    void InitializeScrollButtons();
    void LayoutScrollButtonContainer();

    void InstallHelpContent(const std::shared_ptr<zaf::Control>& control);
    void UpdateWindowHeight();

private:
    ZAF_BIND_CONTROL(zaf::Control, scrollButtonContainer);
    ZAF_BIND_CONTROL(zaf::Control, lineDownButton);
    ZAF_BIND_CONTROL(zaf::Control, lineUpButton);
    ZAF_BIND_CONTROL(zaf::Control, pageDownButton);
    ZAF_BIND_CONTROL(zaf::Control, pageUpButton);

    std::shared_ptr<zaf::ScrollableControl> scroll_control_;
    std::shared_ptr<zaf::Control> help_content_control_;
};

}