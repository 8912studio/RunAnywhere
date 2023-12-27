#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/window/message/keyboard_message.h>
#include <zaf/window/window.h>
#include "module/chat_gpt/dialog.h"

namespace ra::mod::chat_gpt {

class DialogWindow : public zaf::Window {
public:
    ZAF_DECLARE_TYPE;

public:
    explicit DialogWindow(std::shared_ptr<Dialog> dialog);

    void Chat(std::wstring question);

protected:
    void AfterParse() override;
    void OnShow(const zaf::ShowInfo&) override;

private:
    void ResetInputHeight();
    void StartNewRoundOnPressReturn();
    void StartNewRound(std::wstring question);

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, roundScrollControl);
    ZAF_BIND_CONTROL(zaf::Control, roundListView);
    ZAF_BIND_CONTROL(zaf::Control, inputContainer);
    ZAF_BIND_CONTROL(zaf::RichEdit, inputEdit);

    std::shared_ptr<Dialog> dialog_;
};

}