#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/rich_edit.h>
#include "module/command_preview_control.h"
#include "module/common/error_view.h"
#include "module/common/text_content_control.h"

namespace ra::mod {

class TextPreviewControl : public CommandPreviewControl {
public:
    ZAF_OBJECT;

public:
    void SetText(std::wstring text);
    void SetWordWrapping(zaf::WordWrapping word_wrapping);

protected:
    void OnStyleChanged() override;
    zaf::Frame GetExpectedMargin() override;

private:
    ZAF_BIND_CONTROL(TextContentControl, textContent);
    ZAF_BIND_CONTROL(ErrorView, errorView);
};

ZAF_OBJECT_BEGIN(TextPreviewControl)
ZAF_OBJECT_RESOURCE_URI(L"res:///module/common/text_preview_control.xaml")
ZAF_OBJECT_END;

}