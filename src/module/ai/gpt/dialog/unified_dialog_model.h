#pragma once

#include <map>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/subscription_host.h>
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

    const std::shared_ptr<DialogService>& DialogService() const {
        return service_;
    }

    const std::shared_ptr<DialogDataSource>& DialogDataSource() const {
        return dialog_data_source_;
    }

    zaf::Observable<RoundList> FetchRoundsInDialog(DialogID dialog_id);

    std::shared_ptr<Round> CreateNewRound(
        const std::shared_ptr<Dialog>& dialog, 
        std::wstring question,
        RoundList history_rounds);

    void DeleteRound(DialogID dialog_id, RoundID round_id);

private:
    void SubscribeToServiceEvents();
    void OnDialogCreated(const DialogCreatedInfo& event_info);
    void OnDialogPersisted(const DialogPersistedInfo& event_info);
    void OnDialogUpdated(const DialogUpdatedInfo& event_info);

    DialogID MapToPermanentID(DialogID dialog_id) const;
    std::shared_ptr<Dialog> MapToPermanentDialog(const std::shared_ptr<Dialog>& dialog) const;

private:
    std::shared_ptr<gpt::DialogService> service_;

    std::map<DialogTransientID, DialogPermanentID> dialog_permanent_id_map_;
    std::shared_ptr<gpt::DialogDataSource> dialog_data_source_;
};

}