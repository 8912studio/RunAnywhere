#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/control/text_box.h>
#include "module/command_preview_control.h"

namespace ra::module::extension {

class ExtensionCommandPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void SetText(const std::wstring& text);

protected:
    void AfterParse() override;

private:
    void ResetTextBoxHeight();

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::TextBox, textBox);
};

}