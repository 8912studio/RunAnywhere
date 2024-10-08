#pragma once

#include <map>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/create_round_task.h"
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/dialog_service.h"
#include "module/ai/gpt/dialog/id.h"
#include "module/ai/gpt/dialog/list/dialog_data_source.h"
#include "module/ai/gpt/dialog/round.h"

namespace ra::mod::ai::gpt {

class UnifiedDialogModel : zaf::SubscriptionHost {
public:
    explicit UnifiedDialogModel(std::shared_ptr<DialogService> service);

    void Initialize();

    const std::shared_ptr<DialogDataSource>& DialogDataSource() const {
        return dialog_data_source_;
    }

    std::shared_ptr<Dialog> CreateNewDialog();

    zaf::Observable<RoundList> FetchPermanentRoundsInDialog(DialogID dialog_id);

    std::shared_ptr<CreateRoundTask> CreateNewRound(
        std::shared_ptr<Dialog> dialog, 
        std::vector<Message> messages);

    void SubscribeToDialogSavedEvent(const CreateRoundTask& task);
    void SubscribeToDialogUpdatedEvent(const CreateRoundTask& task);

    std::shared_ptr<CreateRoundTask> GetCreateRoundTaskInDialog(DialogID dialog_id);

private:
    std::shared_ptr<DialogService> service_;

    std::map<DialogTransientID, DialogPermanentID> dialog_permanent_id_map_;
    std::shared_ptr<gpt::DialogDataSource> dialog_data_source_;
};

}