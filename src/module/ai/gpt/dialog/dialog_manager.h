#pragma once

#include <map>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/content/round_model.h"
#include "module/ai/gpt/dialog/create_round_task.h"
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/id.h"
#include "module/ai/gpt/dialog/list/dialog_data_source.h"
#include "module/ai/gpt/dialog/round.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

struct CreateNewRoundResult {
    RoundTransientID transient_id{};
    zaf::Observable<ChatResult> chat_result;
};

class DialogManager : zaf::SubscriptionHost {
public:
    DialogManager(
        std::shared_ptr<OpenAIClient> client,
        std::shared_ptr<GPTStorage> storage);

    void Initialize();

    const std::shared_ptr<DialogDataSource>& DialogDataSource() const {
        return dialog_data_source_;
    }

    std::shared_ptr<Dialog> CreateNewDialog();

    zaf::Observable<RoundList> FetchPermanentRoundsInDialog(DialogID dialog_id);

    std::shared_ptr<CreateRoundTask> CreateNewRound(
        std::shared_ptr<const Dialog> dialog, 
        std::vector<Message> messages);

    std::shared_ptr<CreateRoundTask> GetCreateRoundTaskInDialog(DialogID dialog_id);

private:
    zaf::Observable<ChatCompletion> CreateRoundAnswerFromEntity(const RoundEntity& entity);

private:
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<GPTStorage> storage_;

    std::size_t new_dialog_transient_id_{ 1 };
    std::map<DialogTransientID, DialogPermanentID> dialog_permanent_id_map_;
    std::shared_ptr<gpt::DialogDataSource> dialog_data_source_;

    std::size_t new_round_transient_id_{ 1 };
    std::map<DialogID, std::shared_ptr<CreateRoundTask>> create_round_tasks_;
};

}