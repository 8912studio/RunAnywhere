#include "module/ai/gpt/dialog/content/dialog_model.h"
#include <zaf/base/container/utility/erase.h>
#include <zaf/base/container/utility/sort.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/network/response_parsing.h"

namespace ra::mod::ai::gpt {

DialogModel::DialogModel(
    std::shared_ptr<gpt::Dialog> dialog,
    std::shared_ptr<UnifiedDialogModel> unified_dialog_model)
    :
    dialog_(std::move(dialog)),
    unified_dialog_model_(std::move(unified_dialog_model)) {

}


void DialogModel::Initialize() {

    SubscribeToServiceEvents();
    FetchInitialRounds();
}


void DialogModel::SubscribeToServiceEvents() {

    const auto& service = unified_dialog_model_->DialogService();

    Subscriptions() += service->DialogUpdatedEvent().Subscribe(
        std::bind_front(&DialogModel::OnDialogUpdated, this));

    Subscriptions() += service->RoundCreatedEvent().Subscribe(
        std::bind_front(&DialogModel::OnRoundCreated, this));

    Subscriptions() += service->RoundPersistedEvent().Subscribe(
        std::bind_front(&DialogModel::OnRoundPersisted, this));
}


void DialogModel::OnDialogUpdated(const DialogUpdatedInfo& event_info) {

    if (dialog_->ID() == event_info.dialog->ID()) {
        dialog_ = event_info.dialog;
    }
}


void DialogModel::OnRoundCreated(const RoundCreatedInfo& event_info) {

}


void DialogModel::OnRoundPersisted(const RoundPersistedInfo& event_info) {

    if (event_info.dialog_id != dialog_->ID()) {
        return;
    }

    round_permanent_id_map_[event_info.transient_id] = event_info.permanent_id;
}


void DialogModel::FetchInitialRounds() {

    Subscriptions() += unified_dialog_model_->FetchRoundsInDialog(dialog_->ID()).Subscribe(
        [this](const RoundList& rounds) {
    
        round_data_source_.PrependRounds(rounds);
    });
}


std::shared_ptr<Round> DialogModel::CreateRound(std::wstring question) {

    auto new_round = unified_dialog_model_->CreateNewRound(
        dialog_, 
        std::move(question),
        GenerateHistoryRounds());

    round_data_source_.AppendRounds({ new_round });
    return new_round;
}


RoundList DialogModel::GenerateHistoryRounds() const {

    auto round_count = round_data_source_.GetRoundCount();
    if (round_count == 0) {
        return {};
    }

    RoundList result;

    std::size_t index = round_count - 1;
    while (index >= 0) {

        auto round = round_data_source_.GetRoundAtIndex(index);
        if (round->State() == RoundState::Completed) {
            result.push_back(round);
        }

        if (index == 0) {
            break;
        }
        --index;
    }

    std::reverse(result.begin(), result.end());
    return result;
}


void DialogModel::DeleteRound(RoundID id) {

    if (auto transient_id = id.TransientID()) {

        auto iterator = round_permanent_id_map_.find(*transient_id);
        if (iterator != round_permanent_id_map_.end()) {
            unified_dialog_model_->DeleteRound(dialog_->ID(), RoundID{ iterator->second });
            round_permanent_id_map_.erase(iterator);
        }
    }
    else {
        unified_dialog_model_->DeleteRound(dialog_->ID(), id);
    }

    round_data_source_.DeleteAndTakeRound(id);
}


void DialogModel::RetryRound(RoundID id) {

    auto round = round_data_source_.DeleteAndTakeRound(id);
    if (!round) {
        return;
    }

    CreateRound(round->Question());
}

}


