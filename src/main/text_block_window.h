#pragma once

#include <zaf/base/com_object.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include "main/text_block_object.h"
#include "utility/thin_border_window.h"

namespace ra {

class TextBlockWindow : public utility::ThinBorderWindow {
public:
    ZAF_DECLARE_TYPE;

public:
    explicit TextBlockWindow(const zaf::COMObject<TextBlockObject>& object);

protected:
    void AfterParse() override;
    void OnDeactivated(const zaf::DeactivatedInfo& event_info) override;
    void OnMessageReceived(const zaf::MessageReceivedInfo& event_info) override;

private:
    ZAF_BIND_CONTROL(zaf::RichEdit, textEdit);

    zaf::COMObject<TextBlockObject> object_;
};

}

