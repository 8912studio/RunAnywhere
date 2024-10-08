#pragma once

#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/scroll_box.h>
#include <zaf/control/text_box.h>
#include "module/ai/gpt/dialog/content/dialog_model.h"
#include "module/ai/gpt/dialog/content/round_view.h"
#include "utility/composite/composite_text_box.h"

namespace ra::mod::ai::gpt {

class DialogView : public zaf::Control {
public:
    ZAF_OBJECT;

    explicit DialogView(std::unique_ptr<DialogModel> model);

    const DialogModel& Model() const {
        return *model_;
    }

    void Chat(std::wstring question);
    void SetFocusToInputEdit();

    std::wstring Subject() const;
    zaf::Observable<zaf::None> SubjectUpdatedEvent() const;

protected:
    void AfterParse() override;

private:
    void InitializeInputEdit();
    void InitializeSendButton();
    void InitializeRoundListView();

    void ResetControlStates();
    void ResetInputHeight();
    void ResetSendButtonState();

    void LoadRounds();

    void StartNewRoundOnPressReturn();
    void StartNewRound(std::wstring question);
    void SubscribeToAnswerEvent(const Round& round);
    std::shared_ptr<RoundView> CreateRoundView(std::shared_ptr<Round> round);
    void DeleteRound(RoundID round_id);

private:
    ZAF_BIND_CONTROL(zaf::ScrollBox, roundScrollBox);
    ZAF_BIND_CONTROL(utility::composite::CompositeTextBox, roundListView);
    ZAF_BIND_CONTROL(zaf::Control, bottomContainer);
    ZAF_BIND_CONTROL(zaf::TextBox, inputEdit);
    ZAF_BIND_CONTROL(zaf::Button, sendButton);

    std::unique_ptr<DialogModel> model_;
};

ZAF_OBJECT_BEGIN(DialogView);
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/content/dialog_view.xaml");
ZAF_OBJECT_END;

}