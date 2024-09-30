#include "module/ai/gpt/dialog/content/dialog_model.h"
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/network/response_parsing.h"

namespace ra::mod::ai::gpt {

DialogModel::DialogModel(
    std::shared_ptr<Dialog> dialog,
    std::shared_ptr<UnifiedDialogModel> unified_dialog_model)
    :
    dialog_(std::move(dialog)),
    unified_dialog_model_(std::move(unified_dialog_model)) {

}


zaf::Observable<RoundList> DialogModel::FetchRounds() {

    return unified_dialog_model_->FetchPermanentRoundsInDialog(dialog_->ID());
}


std::shared_ptr<Round> DialogModel::CreateRound(std::wstring question) {
    
    Message message{ question };
    auto task = unified_dialog_model_->CreateNewRound(dialog_, { std::move(message) });

    Subscriptions() += task->RoundSavedEvent().Subscribe([this](const RoundSavedInfo& info) {
        round_permanent_id_map_[info.transient_id] = info.permanent_id;
    });

    return std::make_shared<Round>(
        RoundID{ task->GetRoundTransientID() },
        std::move(question),
        task->AnswerEvent());
}

}


