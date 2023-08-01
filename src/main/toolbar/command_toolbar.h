#pragma once

#include <zaf/base/event/event.h>
#include <zaf/base/none.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include "main/command_display_style.h"
#include "main/toolbar/toolbar_button.h"
#include "module/command_executor.h"

namespace ra {

class CommandToolbar : public zaf::HorizontalBox {
public:
    ZAF_DECLARE_TYPE;

    void UpdateStyle(
        CommandDisplayStyle style,
        const std::shared_ptr<mod::CommandExecutor>& executor);

    zaf::Observable<zaf::None> ExecuteEvent() const {
        return execute_event_.GetObservable();
    }

    zaf::Observable<zaf::None> PreserveEvent() const {
        return preserve_event_.GetObservable();
    }

    zaf::Observable<zaf::None> CloseEvent() const {
        return close_event_.GetObservable();
    }

protected:
    void AfterParse() override;

private:
    void UpdateExecuteButton(
        CommandDisplayStyle style, 
        const std::shared_ptr<mod::CommandExecutor>& executor);
    void UpdatePreserveButton(CommandDisplayStyle style);

private:
    ZAF_BIND_CONTROL(ToolbarButton, executeButton);
    ZAF_BIND_CONTROL(ToolbarButton, preserveButton);
    ZAF_BIND_CONTROL(ToolbarButton, closeButton);

    zaf::Event<zaf::None> execute_event_;
    zaf::Event<zaf::None> preserve_event_;
    zaf::Event<zaf::None> close_event_;
};

}