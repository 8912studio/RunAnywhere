#pragma once

#include <zaf/base/none.h>
#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
#include <zaf/rx/subject.h>
#include "module/ai/gpt/dialog/content/answer_view.h"
#include "module/ai/gpt/dialog/round.h"
#include "utility/markdown/render/styled_text_box.h"

namespace ra::mod::ai::gpt {

class RoundView : public zaf::VerticalBox {
public:
    ZAF_OBJECT;

    explicit RoundView(std::shared_ptr<gpt::Round> round);

    std::shared_ptr<gpt::AnswerView> AnswerView() const {
        return answerView;
    }

    const std::shared_ptr<gpt::Round>& Round() const {
        return round_;
    }

    zaf::Observable<RoundID> DeleteEvent() const {
        return delete_event_.AsObservable();
    }

    zaf::Observable<RoundID> RetryEvent() const {
        return retry_event_.AsObservable();
    }

protected:
    void AfterParse() override;
    void OnMouseEnter(const zaf::MouseEnterInfo& event_info) override;
    void OnMouseLeave(const zaf::MouseLeaveInfo& event_info) override;

private:
    void InitializeRoundState();
    void UpdateViewByRoundState();
    void UpdateTokenUsage(const std::optional<TokenUsage>& usage);
    void UpdateToolbarState();

    void SubscribeButtonEvents();

private:
    ZAF_BIND_CONTROL(zaf::Control, questionView);
    ZAF_BIND_CONTROL(zaf::TextBox, questionContent);
    ZAF_BIND_CONTROL(gpt::AnswerView, answerView);
    ZAF_BIND_CONTROL(zaf::Control, toolbar);
    ZAF_BIND_CONTROL(zaf::Button, copyButton);
    ZAF_BIND_CONTROL(zaf::Button, deleteButton);
    ZAF_BIND_CONTROL(zaf::Button, retryButton);
    ZAF_BIND_CONTROL(zaf::Label, tokenUsage);

    std::shared_ptr<gpt::Round> round_;

    zaf::Subject<RoundID> delete_event_;
    zaf::Subject<RoundID> retry_event_;
};

ZAF_OBJECT_BEGIN(RoundView);
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/content/round_view.xaml")
ZAF_OBJECT_END;

}