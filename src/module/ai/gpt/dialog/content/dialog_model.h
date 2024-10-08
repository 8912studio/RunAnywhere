#pragma once

#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/create_round_task.h"
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/unified_dialog_model.h"
#include "module/ai/gpt/dialog/round.h"

namespace ra::mod::ai::gpt {

class DialogModel : zaf::SubscriptionHost, zaf::NonCopyableNonMovable {
public:
    DialogModel(
        std::shared_ptr<Dialog> dialog,
        std::shared_ptr<UnifiedDialogModel> unified_dialog_model);

    const std::shared_ptr<gpt::Dialog>& Dialog() const {
        return dialog_;
    }

    zaf::Observable<RoundList> FetchRounds();

    std::shared_ptr<Round> CreateRound(std::wstring question);

    void DeleteRound(RoundID id);

private:
    std::shared_ptr<Round> CreateTransientRoundFromTask(const CreateRoundTask& task);

private:
    std::shared_ptr<gpt::Dialog> dialog_;
    std::map<RoundTransientID, RoundPermanentID> round_permanent_id_map_;
    
    std::shared_ptr<UnifiedDialogModel> unified_dialog_model_;
};

}