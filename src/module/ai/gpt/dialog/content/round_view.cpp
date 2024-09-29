#include "module/ai/gpt/dialog/content/round_view.h"
#include <format>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/subject.h>
#include "utility/clipboard.h"

using namespace ra::utility::markdown::render;

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(RoundView);

RoundView::RoundView(std::shared_ptr<RoundModel> model) : round_model_(std::move(model)) {

    ZAF_EXPECT(round_model_);
}


void RoundView::AfterParse() {

    __super::AfterParse();

    questionContent->SetText(round_model_->Question());

    answerView->SetAnswer(ObserveAnswer());

    Subscriptions() += copyButton->ClickEvent().Subscribe(std::bind([this]() {
        Subscriptions() += round_model_->Answer().Subscribe([](const ChatCompletion& completion) {
            utility::SetStringToClipboard(completion.Message().Content());
        });
    }));

    Subscriptions() += removeButton->ClickEvent().Subscribe(std::bind([this]() {
        //round_model_->Remove();
    }));

    Subscriptions() += retryButton->ClickEvent().Subscribe(std::bind([this]() {
        //round_model_->Retry();
    }));
}


zaf::Observable<std::wstring> RoundView::ObserveAnswer() {

    ChangeState(RoundState::Requesting);

    return round_model_->Answer().Do([this](const ChatCompletion& completion) {
        UpdateTokenUsage(completion.TokenUsage());
    },
    [this](const std::exception_ptr&) {
        ChangeState(RoundState::Error);
    },
    [this]() {
        ChangeState(RoundState::Finished);
    })
    .Map<std::wstring>([](const ChatCompletion& completion) {
        return completion.Message().Content();
    });
}


void RoundView::OnMouseEnter(const zaf::MouseEnterInfo& event_info) {

    __super::OnMouseEnter(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    toolbar->SetIsVisible(true);
    event_info.MarkAsHandled();
}


void RoundView::OnMouseLeave(const zaf::MouseLeaveInfo& event_info) {

    __super::OnMouseLeave(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!this->ContainsMouse()) {
        toolbar->SetIsVisible(false);
    }
    event_info.MarkAsHandled();
}


void RoundView::UpdateTokenUsage(const std::optional<TokenUsage>& usage) {

    if (!usage.has_value()) {
        tokenUsage->SetIsVisible(false);
        return;
    }

    auto text = std::format(
        L"¡ü {}  ¡ý {}  ¦² {}",
        usage->prompt_tokens,
        usage->completion_tokens,
        usage->total_tokens);

    tokenUsage->SetText(std::move(text));
    tokenUsage->SetTooltip(L"Token usage(question/answer/total)");
    tokenUsage->SetIsVisible(true);
}


void RoundView::ChangeState(RoundState state) {

    state_ = state;
    UpdateToolbarState();
}


void RoundView::UpdateToolbarState() {

    struct ButtonItem {
        zaf::Button& button;
        std::vector<RoundState> visible_states;
    };

    const ButtonItem button_items[] = {
        { *copyButton, { RoundState::Finished } },
        { *removeButton, { RoundState::Finished, RoundState::Error } },
        { *retryButton, {RoundState::Error } },
    };

    for (const auto& each_item : button_items) {
        each_item.button.SetIsVisible(zaf::Contain(each_item.visible_states, state_));
    }
}

}