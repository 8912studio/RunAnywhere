#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/scroll_box.h>
#include <zaf/control/text_box.h>
#include <zaf/window/message/keyboard_message.h>
#include <zaf/window/window.h>
#include "module/chat_gpt/dialog/dialog.h"
#include "utility/composite/composite_text_box.h"

namespace ra::mod::chat_gpt {

class DialogWindow : public zaf::Window {
public:
    ZAF_DECLARE_TYPE;

public:
    explicit DialogWindow(std::shared_ptr<Dialog> dialog);

    void Open();

    void Chat(std::wstring question);

protected:
    void AfterParse() override;
    void OnMessageReceived(const zaf::MessageReceivedInfo& event_info) override;

private:
    void InitializeInputEdit();
    void InitializeRoundListView();

    bool HandleKeyDownMessage(const zaf::KeyMessage& message);

    void ResetInputHeight();
    void OpenWindow();

    void StartNewRoundOnPressReturn();
    void StartNewRound(std::wstring question);
    void SubscribeToAnswerEvent(const Round& round);
    void SubscribeToRoundEvents(const Round& round);
    void RemoveRound(std::uint64_t round_id);

private:
    ZAF_BIND_CONTROL(zaf::ScrollBox, roundScrollBox);
    ZAF_BIND_CONTROL(utility::composite::CompositeTextBox, roundListView);
    ZAF_BIND_CONTROL(zaf::Control, inputContainer);
    ZAF_BIND_CONTROL(zaf::TextBox, inputEdit);

    std::shared_ptr<Dialog> dialog_;
};

}