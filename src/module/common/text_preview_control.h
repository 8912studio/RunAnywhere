#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"
#include "module/common/error_view.h"
#include "module/common/text_content_control.h"

namespace ra::mod {

class TextPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetDisplayMode(TextDisplayMode display_mode);
    void SetText(std::wstring text);

protected:
    zaf::Frame GetExpectedMargin() override;

private:
    ZAF_BIND_CONTROL(TextContentControl, textContent);
    ZAF_BIND_CONTROL(ErrorView, errorView);
};

}