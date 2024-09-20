#pragma once

#include <zaf/base/none.h>
#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
#include "module/ai/gpt/dialog/answer_view.h"
#include "module/ai/gpt/dialog/round.h"
#include "utility/markdown/render/styled_text_box.h"

namespace ra::mod::ai::gpt {

class RoundView : public zaf::VerticalBox {
public:
    ZAF_OBJECT;

    explicit RoundView(std::shared_ptr<Round> round);

    std::shared_ptr<gpt::AnswerView> AnswerView() const {
        return answerView;
    }

    const std::shared_ptr<Round>& Round() const {
        return round_;
    }

protected:
    void AfterParse() override;
    void OnMouseEnter(const zaf::MouseEnterInfo& event_info) override;
    void OnMouseLeave(const zaf::MouseLeaveInfo& event_info) override;

private:
    enum class RoundState {
        Requesting,
        Error,
        Finished,
    };

private:
    zaf::Observable<std::wstring> ObserveAnswer();
    void UpdateTokenUsage(const std::optional<TokenUsage>& usage);
    void ChangeState(RoundState state);
    void UpdateToolbarState();

private:
    ZAF_BIND_CONTROL(zaf::Control, questionView);
    ZAF_BIND_CONTROL(zaf::TextBox, questionContent);
    ZAF_BIND_CONTROL(gpt::AnswerView, answerView);
    ZAF_BIND_CONTROL(zaf::Control, toolbar);
    ZAF_BIND_CONTROL(zaf::Button, copyButton);
    ZAF_BIND_CONTROL(zaf::Button, removeButton);
    ZAF_BIND_CONTROL(zaf::Button, retryButton);
    ZAF_BIND_CONTROL(zaf::Label, tokenUsage);

    std::shared_ptr<gpt::Round> round_;
    RoundState state_{ RoundState::Requesting };
};

ZAF_OBJECT_BEGIN(RoundView);
ZAF_OBJECT_RESOURCE_URI(L"res:///module/ai/gpt/dialog/round_view.xaml")
ZAF_OBJECT_END;

}