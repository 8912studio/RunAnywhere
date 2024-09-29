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


zaf::Observable<RoundModelList> DialogViewController::FetchRounds() {
    return dialog_manager_->FetchAllRoundsInDialog(dialog_->Entity().id);
}


std::shared_ptr<RoundModel> DialogViewController::CreateRound(std::wstring question) {
    
    Message message{ std::move(question) };
    return dialog_manager_->CreateNewRound(dialog_, { std::move(message) });
}

}


