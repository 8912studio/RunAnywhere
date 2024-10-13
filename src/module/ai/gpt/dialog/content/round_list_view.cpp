#include "module/ai/gpt/dialog/content/round_list_view.h"
#include <zaf/base/error/contract_error.h>

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(RoundListView);

RoundListView::RoundListView(std::shared_ptr<DialogModel> model) : model_(std::move(model)) {
    ZAF_EXPECT(model_);
}


void RoundListView::AfterParse() {

    __super::AfterParse();

    InitializeRoundList();
    SubscribeToModelEvents();
    LoadInitialRounds();
}


void RoundListView::InitializeRoundList() {

    Subscriptions() += roundList->SelectionChangedEvent().Subscribe([this](
        const utility::composite::CompositeTextBoxSelectionChangedInfo& event_info) {

        const auto& selection_y = event_info.PositionRange().End().y;

        const auto& vertical_scroll_bar = scrollBox->VerticalScrollBar();
        float visible_y_begin = static_cast<float>(vertical_scroll_bar->Value());
        if (selection_y < visible_y_begin) {
            vertical_scroll_bar->SetValue(static_cast<int>(selection_y));
            return;
        }

        float visible_height = scrollBox->GetVisibleScrollContentRect().size.height;;
        float visible_y_end = visible_y_begin + visible_height;
        if (selection_y >= visible_y_end) {
            vertical_scroll_bar->SetValue(static_cast<int>(selection_y - visible_height));
            return;
        }
    });
}


void RoundListView::SubscribeToModelEvents() {

    const auto& data_source = model_->RoundDataSource();
    Subscriptions() += data_source.RoundPrependedEvent().Subscribe(
        std::bind_front(&RoundListView::OnRoundPrepended, this));

    Subscriptions() += data_source.RoundAppendedEvent().Subscribe(
        std::bind_front(&RoundListView::OnRoundAppended, this));

    Subscriptions() += data_source.RoundDeletedEvent().Subscribe(
        std::bind_front(&RoundListView::OnRoundDeleted, this));
}


void RoundListView::OnRoundPrepended(const RoundPrependedInfo& event_info) {

    if (event_info.count == 0) {
        return;
    }

    auto update_guard = roundList->BeginUpdate();

    const auto& data_source = model_->RoundDataSource();

    std::size_t index = event_info.count - 1;
    while (index >= 0) {

        auto round = data_source.GetRoundAtIndex(index);
        auto round_view = CreateRoundView(std::move(round));
        roundList->AddChildAtIndex(std::move(round_view), 0);

        if (index == 0) {
            break;
        }

        --index;
    }
}


void RoundListView::OnRoundAppended(const RoundAppendedInfo& event_info) {

    {
        auto update_guard = roundList->BeginUpdate();

        const auto& data_source = model_->RoundDataSource();

        for (auto index :
            zaf::Range{ data_source.GetRoundCount() - event_info.count, event_info.count }) {

            auto round = data_source.GetRoundAtIndex(index);
            auto round_view = CreateRoundView(std::move(round));
            roundList->AddChild(std::move(round_view));
        }
    }

    scrollBox->ScrollToBottom();
}


void RoundListView::OnRoundDeleted(const RoundDeletedInfo& event_info) {
    roundList->RemoveChildAtIndex(event_info.index);
}


void RoundListView::LoadInitialRounds() {

    const auto& data_source = model_->RoundDataSource();
    for (auto index : zaf::Range{ 0, data_source.GetRoundCount() }) {

        auto round = data_source.GetRoundAtIndex(index);
        auto round_view = CreateRoundView(std::move(round));
        roundList->AddChild(round_view);
    }
}


std::shared_ptr<RoundView> RoundListView::CreateRoundView(std::shared_ptr<Round> round) {

    //Subscribing to the state changed event should be placed before creating the round view, 
    //as we need to handle the event before any other subscribers.
    if (round->State() == RoundState::Ongoing) {
        Subscriptions() += round->StateChangedEvent().Subscribe(
            std::bind_front(&RoundListView::OnRoundStateChanged, this, round->ID()));
    }

    auto round_view = zaf::Create<RoundView>(std::move(round));

    Subscriptions() += round_view->DeleteEvent().Subscribe(
        std::bind_front(&RoundListView::DeleteRound, this));

    /*
    Subscriptions() += round_view.RetryEvent().Subscribe([this](const std::shared_ptr<Round>& round) {
        DeleteRound(round->ID());
        StartNewRound(round->Question());
    });
    */
    return round_view;
}


void RoundListView::OnRoundStateChanged(RoundID round_id, RoundState new_state) {

    if (new_state != RoundState::Completed) {
        return;
    }

    auto scroll_bar = scrollBox->VerticalScrollBar();
    bool is_list_in_bottom = scroll_bar->Value() == scroll_bar->MaxValue();

    //Don't scroll the list if it isn't in bottom.
    if (!is_list_in_bottom) {
        return;
    }

    const auto& children = roundList->Children();
    if (children.empty()) {
        return;
    }

    //Scroll to the position of answer only if the last round view matches the round id.
    auto last_round_view = zaf::As<RoundView>(children.back());
    if (last_round_view->Round()->ID() == round_id) {

        auto answer_view_position = last_round_view->AnswerView()->TranslateToParent({});
        float scroll_to_position = last_round_view->Y() + answer_view_position.y;
        scrollBox->VerticalScrollBar()->SetValue(static_cast<int>(scroll_to_position));
    }
    //Otherwise, always scroll to the bottom of the list if the list content is changed.
    else {
        scrollBox->ScrollToBottom();
    }
}


void RoundListView::DeleteRound(RoundID round_id) {
    model_->DeleteRound(round_id);
}

}