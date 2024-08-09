#pragma once

#include <zaf/base/event/event.h>
#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include "main/input/command_input_content.h"
#include "main/input/command_input_edit.h"
#include "main/toolbar/command_toolbar.h"
#include "main/toolbar/toolbar_button.h"
#include "module/command.h"
#include "module/command_preview_control.h"
#include "utility/command_line.h"

namespace ra {

class PreservedCommandView : public zaf::Control {
public:
    ZAF_OBJECT;

public:
    PreservedCommandView(
        main::input::CommandInputContent command_input_content,
        std::unique_ptr<mod::Command> command);

    zaf::Observable<std::shared_ptr<PreservedCommandView>> StateUpdatedEvent() const {
        return state_updated_event_.GetObservable();
    }

    zaf::Observable<std::shared_ptr<PreservedCommandView>> CloseEvent() const {
        return close_event_.GetObservable();
    }

protected:
    void AfterParse() override;

private:
    void InitializeToolbar();
    void UpdateCommandState();

private:
    ZAF_BIND_CONTROL(main::input::CommandInputEdit, commandEdit);
    ZAF_BIND_CONTROL(zaf::Control, previewContainer);
    ZAF_BIND_CONTROL(CommandToolbar, toolbar);

    main::input::CommandInputContent command_input_content_;
    std::unique_ptr<mod::Command> command_;

    zaf::Event<std::shared_ptr<PreservedCommandView>> state_updated_event_;
    zaf::Event<std::shared_ptr<PreservedCommandView>> close_event_;
};

ZAF_OBJECT_BEGIN(PreservedCommandView)
ZAF_OBJECT_RESOURCE_URI(L"res:///main/preserve/preserved_command_view.xaml")
ZAF_OBJECT_END;

}