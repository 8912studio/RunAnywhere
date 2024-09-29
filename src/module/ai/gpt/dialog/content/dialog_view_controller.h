#pragma once

#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include "module/ai/gpt/dialog/content/round_model.h"
#include "module/ai/gpt/dialog/dialog_manager.h"

namespace ra::mod::ai::gpt {

class DialogViewController : zaf::NonCopyableNonMovable {
public:
    DialogViewController(
        std::shared_ptr<DialogItemData> dialog, 
        std::shared_ptr<DialogManager> dialog_manager);

    zaf::Observable<RoundModelList> FetchRounds();

    std::shared_ptr<RoundModel> CreateRound(std::wstring question);

private:
    std::shared_ptr<DialogItemData> dialog_;
    std::shared_ptr<DialogManager> dialog_manager_;
};

}