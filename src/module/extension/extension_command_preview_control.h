#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/scroll_box.h>
#include <zaf/control/rich_edit.h>
#include "module/command_preview_control.h"

namespace ra::mod::extension {

class ExtensionCommandPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    void SetText(const std::wstring& text);

protected:
    void AfterParse() override;

private:
    void ResetTextBoxHeight();

private:
    ZAF_BIND_CONTROL(zaf::ScrollBox, scrollControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, textBox);
};

}