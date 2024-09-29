#pragma once

#include <map>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/content/round_model.h"
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/list/dialog_data_source.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

class DialogManager : zaf::SubscriptionHost {
public:
    DialogManager(
        std::shared_ptr<OpenAIClient> client,
        std::shared_ptr<GPTStorage> storage);

    void Initialize();

    const std::shared_ptr<DialogDataSource>& DialogDataSource() const {
        return dialog_data_source_;
    }

    std::shared_ptr<DialogItemData> CreateNewDialog();

    zaf::Observable<RoundModelList> FetchAllRoundsInDialog(DialogID dialog_id);

    std::shared_ptr<RoundModel> CreateNewRound(
        std::shared_ptr<DialogItemData> dialog,
        std::vector<Message> messages);

private:
    zaf::Observable<ChatCompletion> CreateRoundAnswerFromEntity(const RoundEntity& entity);

    zaf::Observable<ChatCompletion> CreateRoundAnswer(
        std::shared_ptr<DialogItemData> dialog,
        std::vector<Message> messages);

private:
    struct OngoingRoundInfo {
        RoundTransientID transient_id;
        std::optional<zaf::Observable<ChatCompletion>> answer; 
    };

private:
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<GPTStorage> storage_;

    std::size_t new_dialog_transient_id_{ 1 };
    std::shared_ptr<gpt::DialogDataSource> dialog_data_source_;

    std::size_t new_round_transient_id_{ 1 };
    std::map<DialogID, OngoingRoundInfo> ongoing_rounds_;
};

}