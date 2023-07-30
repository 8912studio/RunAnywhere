#pragma once

#include <zaf/base/event/event.h>
#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include "main/command_input_content.h"
#include "main/command_input_edit.h"
#include "module/command.h"
#include "module/command_preview_control.h"
#include "utility/command_line.h"
#include "utility/tool_bar_button.h"

namespace ra {

class HistoryCommandView : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

public:
    HistoryCommandView(
        CommandInputContent command_input_content,
        std::unique_ptr<mod::Command> command);

    zaf::Observable<std::shared_ptr<HistoryCommandView>> CloseEvent() {
        return close_event_.GetObservable();
    }

protected:
    void AfterParse() override;

private:
    ZAF_BIND_CONTROL(CommandInputEdit, commandEdit);
    ZAF_BIND_CONTROL(zaf::Control, previewContainer);
    ZAF_BIND_CONTROL(zaf::Control, toolBar);
    ZAF_BIND_CONTROL(utility::ToolBarButton, closeButton);

private:
    CommandInputContent command_input_content_;
    std::unique_ptr<mod::Command> command_;

    zaf::Event<std::shared_ptr<HistoryCommandView>> close_event_;
};

}