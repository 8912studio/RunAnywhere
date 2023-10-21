#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/window/window.h>
#include "help/help_content.h"
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/markdown_region.h"
#include "utility/thin_border_window.h"

namespace ra::help {

class HelpWindow : public utility::ThinBorderWindow {
public:
    ZAF_DECLARE_TYPE;

    void SetContent(const HelpContent& content);

    void SetContent(std::wstring content_id, const markdown::element::Element& content);

    void ScrollLine(bool scroll_up);
    void ScrollPage(bool scroll_up);

protected:
    void AfterParse() override;

private:
    void InitializeScrollControls();
    void InitializeScrollButtonContainer();
    void InitializeScrollButtons();
    void LayoutScrollButtonContainer();

    void UpdateWindowHeight();
    void OnScroll();

private:
    ZAF_BIND_CONTROL(zaf::Control, scrollButtonContainer);
    ZAF_BIND_CONTROL(zaf::Control, lineDownButton);
    ZAF_BIND_CONTROL(zaf::Control, lineUpButton);
    ZAF_BIND_CONTROL(zaf::Control, pageDownButton);
    ZAF_BIND_CONTROL(zaf::Control, pageUpButton);

    std::shared_ptr<zaf::ScrollableControl> scroll_control_;
    std::shared_ptr<markdown::render::MarkdownRegion> markdown_region_;
    std::wstring content_id_;
    bool is_setting_content_{};
    std::wstring last_scroll_content_id_;
    std::optional<int> last_scroll_value_;
};

}