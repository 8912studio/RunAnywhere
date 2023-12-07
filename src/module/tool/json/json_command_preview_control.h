#pragma once

#include <zaf/control/control_binder.h>
#include "module/command_preview_control.h"
#include "utility/markdown/render/styled_text_box.h"

namespace ra::mod::tool::json {

class JSONCommandPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void SetText(const utility::markdown::render::StyledText& text);

protected:
    void AfterParse() override;
    void Layout(const zaf::Rect&) override;
    void OnStyleChanged() override;
    zaf::Frame GetExpectedMargin() override;
    
private:
    void ResetHeight();

private:
    ZAF_BIND_CONTROL(zaf::Control, scrollControl);
    ZAF_BIND_CONTROL(utility::markdown::render::StyledTextBox, contentControl);
};

}