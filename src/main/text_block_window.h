#pragma once

#include <zaf/base/com_object.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include "utility/thin_border_window.h"

namespace ra {

class TextBlockWindow : public utility::ThinBorderWindow {
public:
    ZAF_DECLARE_TYPE;

public:
    TextBlockWindow();

    void SetText(const std::wstring& text);
    void ResizeToSuitableSize();

protected:
    void AfterParse() override;
    void OnDeactivated(const zaf::DeactivatedInfo& event_info) override;
    void OnMessageReceived(const zaf::MessageReceivedInfo& event_info) override;

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollableControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, textEdit);
};

}

