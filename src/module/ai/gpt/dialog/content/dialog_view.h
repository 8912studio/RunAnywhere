#pragma once

#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/scroll_box.h>
#include <zaf/control/text_box.h>
#include "module/ai/gpt/dialog/content/dialog_model.h"
#include "module/ai/gpt/dialog/content/round_list_view.h"
#include "module/ai/gpt/dialog/content/round_view.h"
#include "utility/composite/composite_text_box.h"

namespace ra::mod::ai::gpt {

class DialogView : public zaf::Control {
public:
    ZAF_OBJECT;

    explicit DialogView(std::shared_ptr<DialogModel> model);

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
    void InitializeRoundListView();
    void InitializeInputEdit();
    void InitializeSendButton();

    void ResetControlStates();
    void ResetInputHeight();
    void ResetSendButtonState();

    void StartNewRoundOnPressReturn();
    void StartNewRound(std::wstring question);

private:
    ZAF_BIND_CONTROL(zaf::Control, roundListContainer);
    ZAF_BIND_CONTROL(zaf::Control, bottomContainer);
    ZAF_BIND_CONTROL(zaf::TextBox, inputEdit);
    ZAF_BIND_CONTROL(zaf::Button, sendButton);

    std::shared_ptr<RoundListView> round_list_view_;
    std::shared_ptr<DialogModel> model_;
};

ZAF_OBJECT_BEGIN(DialogView);
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/content/dialog_view.xaml");
ZAF_OBJECT_END;

}