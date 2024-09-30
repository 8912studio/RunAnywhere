#include "module/ai/gpt/dialog/unified_dialog_model.h"
#include <zaf/base/container/utility/find.h>
#include <zaf/base/error/contract_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>

namespace ra::mod::ai::gpt {

UnifiedDialogModel::UnifiedDialogModel(std::shared_ptr<DialogService> service) :
    service_(std::move(service)),
    dialog_data_source_(std::make_shared<gpt::DialogDataSource>()) {

}


void UnifiedDialogModel::Initialize() {

    Subscriptions() += service_->FetchPermanentDialogs().Subscribe(
        [this](const DialogList& dialogs) {
            dialog_data_source_->AddDialogs(dialogs);
        });
}


std::shared_ptr<Dialog> UnifiedDialogModel::CreateNewDialog() {

    auto new_dialog = service_->CreateNewDialog();
    dialog_data_source_->AddDialog(new_dialog);
    return new_dialog;
}


zaf::Observable<RoundList> UnifiedDialogModel::FetchPermanentRoundsInDialog(DialogID dialog_id) {

    DialogPermanentID dialog_permanent_id{};
    if (auto transient_id = dialog_id.TransientID()) {
        auto permanent_id = zaf::Find(dialog_permanent_id_map_, *transient_id);
        if (permanent_id) {
            dialog_permanent_id = *permanent_id;
        }
    }
    else if (auto permanent_id = dialog_id.PermanentID()) {
        dialog_permanent_id = *permanent_id;
    }

    if (dialog_permanent_id.Value() == 0) {
        return zaf::rx::Just(RoundList{});
    }

    return service_->FetchPermanentRoundsInDialog(dialog_permanent_id);
}


std::shared_ptr<CreateRoundTask> UnifiedDialogModel::CreateNewRound(
    std::shared_ptr<Dialog> dialog,
    std::vector<Message> messages) {

    auto task = service_->CreateNewRound(dialog, std::move(messages));

    Subscriptions() += task->DialogSavedEvent().Subscribe([this](const DialogSavedInfo& info) {
        dialog_permanent_id_map_[info.transient_id] = info.permanent_id;
    });
    
    return task;
}


std::shared_ptr<CreateRoundTask> UnifiedDialogModel::GetCreateRoundTaskInDialog(
    DialogID dialog_id) {

    return service_->GetCreateRoundTaskInDialog(dialog_id);
}

}