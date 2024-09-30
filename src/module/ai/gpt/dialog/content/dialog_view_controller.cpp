#include "module/ai/gpt/dialog/content/dialog_view_controller.h"
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/network/response_parsing.h"

namespace ra::mod::ai::gpt {

DialogViewController::DialogViewController(
    std::shared_ptr<DialogItemData> dialog,
    std::shared_ptr<DialogManager> dialog_manager) 
    :
    dialog_(std::move(dialog)),
    dialog_manager_(std::move(dialog_manager)) {

}


zaf::Observable<RoundList> DialogViewController::FetchRounds() {


}


std::shared_ptr<Round> DialogViewController::CreateRound(std::wstring question) {
    
    Message message{ question };
    auto task = dialog_manager_->CreateNewRound(dialog_, { std::move(message) });

    Subscriptions() += task->RoundSavedEvent().Subscribe([this](const RoundSavedInfo& info) {
        round_permanent_id_map_[info.transient_id] = info.permanent_id;
    });

    return std::make_shared<Round>(
        task->GetRoundTransientID(), 
        std::move(question),
        task->AnswerEvent());
}

}


