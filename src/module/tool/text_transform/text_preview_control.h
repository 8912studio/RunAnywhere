#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"

namespace ra::module::tool::text_transform {

class TextPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetText(const std::wstring& text);

protected:
    void AfterParse() override;

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, richEdit);
};

}