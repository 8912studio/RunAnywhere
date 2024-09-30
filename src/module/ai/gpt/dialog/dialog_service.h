#pragma once

#include <map>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/create_round_task.h"
#include "module/ai/gpt/dialog/round.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

class DialogService : zaf::SubscriptionHost, zaf::NonCopyableNonMovable {
public:
    DialogService(
        std::shared_ptr<OpenAIClient> client, 
        std::shared_ptr<GPTStorage> storage);

    zaf::Observable<DialogList> FetchPermanentDialogs();
    std::shared_ptr<Dialog> CreateNewDialog();
    
    zaf::Observable<RoundList> FetchPermanentRoundsInDialog(DialogPermanentID dialog_permanent_id);

    std::shared_ptr<CreateRoundTask> CreateNewRound(
        std::shared_ptr<Dialog> dialog,
        std::vector<Message> messages);

    std::shared_ptr<CreateRoundTask> GetCreateRoundTaskInDialog(DialogID dialog_id);

private:
    zaf::Observable<ChatCompletion> CreateRoundAnswerFromEntity(const RoundEntity& entity);

private:
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<GPTStorage> storage_;

    std::size_t new_dialog_transient_id_{ 1 };

    std::size_t new_round_transient_id_{ 1 };
    std::map<DialogID, std::shared_ptr<CreateRoundTask>> create_round_tasks_;
};

}