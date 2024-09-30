#pragma once

#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/dialog_manager.h"
#include "module/ai/gpt/dialog/round.h"

namespace ra::mod::ai::gpt {

class DialogViewController : zaf::SubscriptionHost, zaf::NonCopyableNonMovable {
public:
    DialogViewController(
        std::shared_ptr<DialogItemData> dialog, 
        std::shared_ptr<DialogManager> dialog_manager);

    zaf::Observable<RoundList> FetchRounds();

    std::shared_ptr<Round> CreateRound(std::wstring question);

private:
    std::shared_ptr<Dialog> dialog_;
    std::map<RoundTransientID, RoundPermanentID> round_permanent_id_map_;
    
    std::shared_ptr<DialogManager> dialog_manager_;
};

}