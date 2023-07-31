#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include "main/command_display_style.h"
#include "main/toolbar/toolbar_button.h"

namespace ra {

class CommandToolbar : public zaf::HorizontalBox {
public:
    ZAF_DECLARE_TYPE;

    void SetStyle(CommandDisplayStyle style);

protected:
    void AfterParse() override;

private:
    void UpdateButtons();

private:
    ZAF_BIND_CONTROL(ToolbarButton, executeButton);
    ZAF_BIND_CONTROL(ToolbarButton, preserveButton);
    ZAF_BIND_CONTROL(ToolbarButton, closeButton);

    CommandDisplayStyle style_{ CommandDisplayStyle::Normal };
};

}