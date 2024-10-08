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
        [this](const DialogList& permanent_dialogs) {

            DialogList dialogs = permanent_dialogs;

            auto create_round_tasks = service_->GetAllCreateRoundTasks();
            for (const auto& each_task : create_round_tasks) {

                auto dialog_permanent_id = each_task->GetDialogPermanentID();
                if (!dialog_permanent_id) {

                    auto dialog_index = dialog_data_source_->GetIndexOfDialog(
                        each_task->GetDialog()->ID());

                    if (!dialog_index) {
                        dialogs.push_back(each_task->GetDialog());
                    }

                    SubscribeToDialogSavedEvent(*each_task);
                }
                SubscribeToDialogUpdatedEvent(*each_task);
            }

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
    SubscribeToDialogSavedEvent(*task);
    SubscribeToDialogUpdatedEvent(*task);
    return task;
}


void UnifiedDialogModel::SubscribeToDialogSavedEvent(const CreateRoundTask& task) {

    Subscriptions() += task.DialogSavedEvent().Subscribe([this](const DialogSavedInfo& info) {
        dialog_permanent_id_map_[info.transient_id] = info.permanent_id;
    });
}


void UnifiedDialogModel::SubscribeToDialogUpdatedEvent(const CreateRoundTask& task) {

    Subscriptions() += task.DialogUpdatedEvent().Subscribe([this](const DialogUpdatedInfo& info) {
        dialog_data_source_->UpdateDialog(info.dialog);
    });
}


std::shared_ptr<CreateRoundTask> UnifiedDialogModel::GetCreateRoundTaskInDialog(
    DialogID dialog_id) {

    return service_->GetCreateRoundTaskInDialog(dialog_id);
}


void UnifiedDialogModel::DeleteRound(RoundID round_id) {
    service_->DeleteRound(round_id);
}

}