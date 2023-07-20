#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include "utility/command_line.h"
#include "module/command.h"
#include "module/command_preview_control.h"

namespace ra {

class HistoryCommandView : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

public:
    HistoryCommandView(utility::CommandLine command_line, std::unique_ptr<mod::Command> command);

protected:
    void AfterParse() override;

private:
    ZAF_BIND_CONTROL(zaf::RichEdit, commandEdit);
    ZAF_BIND_CONTROL(zaf::Control, previewContainer);

private:
    utility::CommandLine command_line_;
    std::unique_ptr<mod::Command> command_;
};

}