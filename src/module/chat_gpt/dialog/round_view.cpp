#include "module/chat_gpt/dialog/round_view.h"
#include <zaf/base/container/utility/contain.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/subject.h>
#include "utility/clipboard.h"

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(RoundView);
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/dialog/round_view.xaml")
ZAF_DEFINE_TYPE_END;

RoundView::RoundView(std::shared_ptr<Dialog> dialog) : dialog_(std::move(dialog)) {

}


void RoundView::AfterParse() {

    __super::AfterParse();

    Subscriptions() += copyButton->ClickEvent().Subscribe(std::bind([this]() {
        utility::SetStringToClipboard(answer_);
    }));

    Subscriptions() += removeButton->ClickEvent().Subscribe(std::bind([this]() {

    }));
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


zaf::Observable<zaf::None> RoundView::Start(std::wstring question) {

    ChangeState(RoundState::Requesting);

    zaf::Subject<zaf::None> finish_subject;

    questionContent->SetText(question);

    auto observable = dialog_->Chat(std::move(question)).Map<std::wstring>(
        [](const comm::ChatCompletion& chat_completion) {
        return chat_completion.Message().Content();
    })
    .Do([this](const std::wstring& answer) {
        answer_ = answer;
    },
    [this](const zaf::Error&) {
        ChangeState(RoundState::Error);
    },
    [this]() {
        ChangeState(RoundState::Finished);
    })
    .DoOnTerminated([finish_observer = finish_subject.AsObserver()]() {
        finish_observer.OnNext({});
    });

    answerView->SetAnswer(observable);
    return finish_subject.AsObservable();
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

    static const ButtonItem button_items[] = {
        { *copyButton, { RoundState::Finished } },
        { *removeButton, { RoundState::Finished, RoundState::Error } },
    };

    for (const auto& each_item : button_items) {
        each_item.button.SetIsVisible(zaf::Contain(each_item.visible_states, state_));
    }
}

}