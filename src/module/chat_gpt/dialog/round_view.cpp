#include "module/chat_gpt/dialog/round_view.h"
#include <zaf/base/container/utility/contain.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/subject.h>
#include "utility/clipboard.h"

using namespace ra::utility::markdown::render;

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(RoundView);
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/dialog/round_view.xaml")
ZAF_DEFINE_TYPE_END;

RoundView::RoundView(std::shared_ptr<chat_gpt::Round> round) : round_(std::move(round)) {

    ZAF_EXPECT(round_);
}


void RoundView::AfterParse() {

    __super::AfterParse();

    TextStyle text_style;
    text_style.font = questionContent->Font();
    text_style.text_color = zaf::Color::Black();

    StyledText styled_text;
    styled_text.Append(round_->Question());
    styled_text.AddStyleToPendingText(std::move(text_style));

    questionContent->SetStyledText(styled_text);

    answerView->SetAnswer(ObserveAnswer());

    Subscriptions() += copyButton->ClickEvent().Subscribe(std::bind([this]() {
        Subscriptions() += round_->Answer().Subscribe([](const std::wstring& content) {
            utility::SetStringToClipboard(content);
        });
    }));

    Subscriptions() += removeButton->ClickEvent().Subscribe(std::bind([this]() {
        round_->Remove();
    }));

    Subscriptions() += retryButton->ClickEvent().Subscribe(std::bind([this]() {
        round_->Retry();
    }));
}


zaf::Observable<std::wstring> RoundView::ObserveAnswer() {

    ChangeState(RoundState::Requesting);
    return round_->Answer().Do([](const std::wstring&) {
        //Nothing to do.
    },
    [this](const zaf::Error&) {
        ChangeState(RoundState::Error);
    },
    [this]() {
        ChangeState(RoundState::Finished);
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

    if (!this->ContainMouse()) {
        toolbar->SetIsVisible(false);
    }
    event_info.MarkAsHandled();
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