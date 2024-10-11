#include "module/ai/gpt/dialog/content/round_view.h"
#include <format>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/subject.h>
#include "utility/clipboard.h"

using namespace ra::utility::markdown::render;

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(RoundView);

RoundView::RoundView(std::shared_ptr<gpt::Round> round) : round_(std::move(round)) {
    ZAF_EXPECT(round_);
}


void RoundView::AfterParse() {

    __super::AfterParse();

    questionContent->SetText(round_->Question());
    answerView->SetRound(round_);

    InitializeRoundState();
    SubscribeButtonEvents();
}


void RoundView::InitializeRoundState() {

    UpdateViewByRoundState();

    Subscriptions() += round_->StateChangedEvent().Subscribe([this](RoundState) {
        UpdateViewByRoundState();
    });
}


void RoundView::UpdateViewByRoundState() {

    if (round_->State() == RoundState::Completed) {
        UpdateTokenUsage(round_->Answer().TokenUsage());
    }

    UpdateToolbarState();
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


void RoundView::UpdateToolbarState() {

    struct ButtonItem {
        zaf::Button& button;
        std::vector<RoundState> visible_states;
    };

    const ButtonItem button_items[] = {
        { *copyButton, { RoundState::Completed } },
        { *deleteButton, { RoundState::Completed, RoundState::Error } },
        { *retryButton, {RoundState::Error } },
    };

    for (const auto& each_item : button_items) {
        each_item.button.SetIsVisible(zaf::Contain(each_item.visible_states, round_->State()));
    }
}


void RoundView::SubscribeButtonEvents() {

    Subscriptions() += copyButton->ClickEvent().Subscribe(std::bind([this]() {
        utility::SetStringToClipboard(round_->Answer().Message().Content());
    }));

    Subscriptions() += deleteButton->ClickEvent().Subscribe(std::bind([this]() {
        delete_event_.AsObserver().OnNext(round_->ID());
    }));

    Subscriptions() += retryButton->ClickEvent().Subscribe(std::bind([this]() {
        retry_event_.AsObserver().OnNext(round_->ID());
    }));
}

}