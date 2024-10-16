#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/scroll_box.h>
#include "module/ai/gpt/dialog/content/dialog_model.h"
#include "module/ai/gpt/dialog/content/round_view.h"
#include "utility/composite/composite_text_box.h"

namespace ra::mod::ai::gpt {

class RoundListView : public zaf::Control {
public:
    ZAF_OBJECT;

    explicit RoundListView(std::shared_ptr<DialogModel> model);

protected:
    void AfterParse() override;

private:
    void InitializeRoundList();

    void SubscribeToModelEvents();
    void OnRoundPrepended(const RoundPrependedInfo& event_info);
    void OnRoundAppended(const RoundAppendedInfo& event_info);
    void OnRoundDeleted(const RoundDeletedInfo& event_info);

    void LoadInitialRounds();
    std::shared_ptr<RoundView> CreateRoundView(std::shared_ptr<Round> round);
    void OnRoundStateChanged(RoundID round_id, RoundState new_state);

    void DeleteRound(RoundID round_id);
    void RetryRound(RoundID round_id);

private:
    ZAF_BIND_CONTROL(zaf::ScrollBox, scrollBox);
    ZAF_BIND_CONTROL(utility::composite::CompositeTextBox, roundList);

    std::shared_ptr<DialogModel> model_;
};

ZAF_OBJECT_BEGIN(RoundListView);
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/content/round_list_view.xaml");
ZAF_OBJECT_END;

}