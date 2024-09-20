#include "module/ai/gpt/dialog/dialog_view.h"
#include <zaf/input/keyboard.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/dialog/round_view.h"

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(DialogView);

DialogView::DialogView(std::shared_ptr<Dialog> dialog) : dialog_(std::move(dialog)) {

    ZAF_EXPECT(dialog_);
}


void DialogView::AfterParse() {

    __super::AfterParse();

    InitializeInputEdit();
    InitializeSendButton();
    InitializeRoundListView();
    ResetControlStates();
}


void DialogView::InitializeInputEdit() {

    Subscriptions() += inputEdit->TextChangedEvent().Subscribe(
        std::bind(&DialogView::ResetControlStates, this));

    Subscriptions() += inputEdit->RectChangedEvent().Subscribe(
        [this](const zaf::RectChangedInfo& event_info) {

        if (inputEdit->Width() != event_info.PreviousRect().size.width) {
            ResetInputHeight();
        }
    });

    Subscriptions() += inputEdit->KeyDownEvent().Subscribe([this](
        const zaf::KeyDownInfo& event_info) {

        if (event_info.Message().Key() == zaf::Key::Enter && !zaf::Keyboard::IsShiftDown()) {
            StartNewRoundOnPressReturn();
            event_info.MarkAsHandled();
        }
    });
}


void DialogView::InitializeSendButton() {

    Subscriptions() += sendButton->ClickEvent().Subscribe(
        std::bind(&DialogView::StartNewRoundOnPressReturn, this));
}


void DialogView::InitializeRoundListView() {

    Subscriptions() += roundListView->SelectionChangedEvent().Subscribe([this](
        const utility::composite::CompositeTextBoxSelectionChangedInfo& event_info) {

        const auto& selection_y = event_info.PositionRange().End().y;

        const auto& vertical_scroll_bar = roundScrollBox->VerticalScrollBar();
        float visible_y_begin = static_cast<float>(vertical_scroll_bar->Value());
        if (selection_y < visible_y_begin) {
            vertical_scroll_bar->SetValue(static_cast<int>(selection_y));
            return;
        }

        float visible_height = roundScrollBox->GetVisibleScrollContentRect().size.height;;
        float visible_y_end = visible_y_begin + visible_height;
        if (selection_y >= visible_y_end) {
            vertical_scroll_bar->SetValue(static_cast<int>(selection_y - visible_height));
            return;
        }
    });
}


void DialogView::ResetControlStates() {

    ResetInputHeight();
    ResetSendButtonState();
}


void DialogView::ResetInputHeight() {

    auto line_count = inputEdit->LineCount();
    auto text_height = inputEdit->CalculatePreferredSize(inputEdit->Size()).height;

    auto line_height = std::ceil(text_height / line_count);
    constexpr std::size_t max_line_count = 5;

    auto show_line_count = std::min(line_count, max_line_count);
    auto expected_height = show_line_count * line_height + inputEdit->Margin().Height();
    bottomContainer->SetFixedHeight(expected_height);
}


void DialogView::ResetSendButtonState() {

    sendButton->SetIsEnabled(inputEdit->TextLength() > 0);
}


void DialogView::StartNewRoundOnPressReturn() {

    auto question = inputEdit->Text();
    if (question.empty()) {
        return;
    }

    inputEdit->SetText({});

    StartNewRound(std::move(question));
}


void DialogView::StartNewRound(std::wstring question) {

    auto round = dialog_->CreateRound(std::move(question));

    //We have to subscribe to the answer event before creating the round view, as we need to record
    //the scroll bar state before updating the answer content when the answer event is raised.
    SubscribeToAnswerEvent(*round);
    SubscribeToRoundEvents(*round);

    auto round_view = zaf::Create<RoundView>(round);
    roundListView->AddChild(round_view);
    roundScrollBox->ScrollToBottom();
}


void DialogView::SubscribeToAnswerEvent(const Round& round) {

    auto is_list_in_bottom = std::make_shared<bool>();

    Subscriptions() += round.Answer().Catch([](const std::exception_ptr&) {
        return zaf::rx::Just(ChatCompletion{ Message{ L"" }, {}});
    })
    .Do(std::bind([this, is_list_in_bottom]() {
        auto scroll_bar = roundScrollBox->VerticalScrollBar();
        *is_list_in_bottom = scroll_bar->Value() == scroll_bar->MaxValue();
    }))
    .DoOnTerminated([this, is_list_in_bottom, round_id = round.ID()]() {

        //Don't scroll the list if it isn't in bottom.
        if (!*is_list_in_bottom) {
            return;
        }

        const auto& children = roundListView->Children();
        if (children.empty()) {
            return;
        }

        //Scroll to the position of answer only if the last round view matches the round id.
        auto last_round_view = zaf::As<RoundView>(children.back());
        if (last_round_view->Round()->ID() == round_id) {

            auto answer_view_position = last_round_view->AnswerView()->TranslateToParent({});
            float scroll_to_position = last_round_view->Y() + answer_view_position.y;
            roundScrollBox->VerticalScrollBar()->SetValue(static_cast<int>(scroll_to_position));
        }
        //Otherwise, always scroll to the bottom of the list if the list content is changed.
        else {
            roundScrollBox->ScrollToBottom();
        }
    })
    .Subscribe();
}


void DialogView::SubscribeToRoundEvents(const Round& round) {

    Subscriptions() += round.RemoveEvent().Subscribe(
        std::bind(&DialogView::RemoveRound, this, std::placeholders::_1));

    Subscriptions() += round.RetryEvent().Subscribe([this](const std::shared_ptr<Round>& round) {
        RemoveRound(round->ID());
        StartNewRound(round->Question());
    });
}


void DialogView::RemoveRound(std::uint64_t round_id) {

    const auto& children = roundListView->Children();
    for (auto index : zaf::Range(0, children.size())) {

        if (zaf::As<RoundView>(children[index])->Round()->ID() == round_id) {
            roundListView->RemoveChildAtIndex(index);
            break;
        }
    }
}


void DialogView::Chat(std::wstring question) {

    if (question.empty()) {
        return;
    }

    StartNewRound(std::move(question));
}


void DialogView::SetFocusToInputEdit() {
    inputEdit->SetIsFocused(true);
}


std::wstring DialogView::Subject() const {
    return dialog_->Subject();
}


zaf::Observable<zaf::None> DialogView::SubjectUpdatedEvent() const {
    return dialog_->SubjectUpdatedEvent();
}

}