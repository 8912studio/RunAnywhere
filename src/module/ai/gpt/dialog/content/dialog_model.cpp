#include "module/ai/gpt/dialog/content/dialog_model.h"
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


zaf::Observable<RoundList> DialogModel::FetchRounds() {

    auto permanent_rounds = unified_dialog_model_->FetchPermanentRoundsInDialog(dialog_->ID());

    auto create_round_task = unified_dialog_model_->GetCreateRoundTaskInDialog(dialog_->ID());
    if (!create_round_task) {
        return permanent_rounds;
    }

    return permanent_rounds.Map<RoundList>([this, create_round_task](const RoundList& rounds) {
    
        auto permanent_id = create_round_task->GetRoundPermanentID();
        if (!permanent_id) {

            auto result = rounds;
            auto new_round = CreateTransientRoundFromTask(*create_round_task);
            result.push_back(new_round);
            return result;
        }

        RoundList result;
        for (const auto& each_round : rounds) {

            if (*each_round->ID().PermanentID() == permanent_id) {

                auto new_round = std::make_shared<Round>(
                    each_round->ID(),
                    each_round->Question(),
                    create_round_task->AnswerEvent());

                result.push_back(std::move(new_round));
            }
            else {
                result.push_back(each_round);
            }
        }

        return result;
    });
}


std::shared_ptr<Round> DialogModel::CreateRound(std::wstring question) {
    
    Message message{ question };
    auto task = unified_dialog_model_->CreateNewRound(dialog_, { std::move(message) });

    return CreateTransientRoundFromTask(*task);
}


std::shared_ptr<Round> DialogModel::CreateTransientRoundFromTask(const CreateRoundTask& task) {

    Subscriptions() += task.RoundSavedEvent().Subscribe([this](const RoundSavedInfo& info) {
        round_permanent_id_map_[info.transient_id] = info.permanent_id;
    });

    return std::make_shared<Round>(
        RoundID{ task.GetRoundTransientID() },
        task.GetQuestion(),
        task.AnswerEvent());
}

}


