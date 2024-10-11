#include "module/ai/gpt/dialog/unified_dialog_model.h"
#include <zaf/base/container/utility/find.h>
#include <zaf/base/error/contract_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>

namespace ra::mod::ai::gpt {

UnifiedDialogModel::UnifiedDialogModel(std::shared_ptr<gpt::DialogService> service) :
    service_(std::move(service)),
    dialog_data_source_(std::make_shared<gpt::DialogDataSource>()) {

}


void UnifiedDialogModel::Initialize() {

    SubscribeToServiceEvents();

    Subscriptions() += service_->FetchDialogs().Subscribe([this](const DialogList& dialogs) {

            DialogList new_dialogs;
            for (const auto& each_dialog : dialogs) {

                //Do not add dialogs which are already in the data source.
                if (each_dialog->ID().TransientID()) {
                    auto dialog_index = dialog_data_source_->GetIndexOfDialog(each_dialog->ID());
                    if (dialog_index) {
                        continue;
                    }
                }

                new_dialogs.push_back(each_dialog);
            }

            dialog_data_source_->AddDialogs(std::move(new_dialogs));
        });
}


void UnifiedDialogModel::SubscribeToServiceEvents() {

    Subscriptions() += service_->DialogCreatedEvent().Subscribe(
        std::bind_front(&UnifiedDialogModel::OnDialogCreated, this));

    Subscriptions() += service_->DialogPersistedEvent().Subscribe(
        std::bind_front(&UnifiedDialogModel::OnDialogPersisted, this));

    Subscriptions() += service_->DialogUpdatedEvent().Subscribe(
        std::bind_front(&UnifiedDialogModel::OnDialogUpdated, this));
}


void UnifiedDialogModel::OnDialogCreated(const DialogCreatedInfo& event_info) {
    dialog_data_source_->AddDialog(event_info.dialog);
}


void UnifiedDialogModel::OnDialogPersisted(const DialogPersistedInfo& event_info) {

    ZAF_EXPECT(!dialog_permanent_id_map_.contains(event_info.transient_id));
    dialog_permanent_id_map_[event_info.transient_id] = event_info.permanent_id;
}


void UnifiedDialogModel::OnDialogUpdated(const DialogUpdatedInfo& event_info) {
    dialog_data_source_->UpdateDialog(event_info.dialog);
}


zaf::Observable<RoundList> UnifiedDialogModel::FetchRoundsInDialog(DialogID dialog_id) {
    return service_->FetchRoundsInDialog(MapToPermanentID(dialog_id));
}


std::shared_ptr<Round> UnifiedDialogModel::CreateNewRound(
    const std::shared_ptr<Dialog>& dialog,
    std::wstring question,
    RoundList history_rounds) {

    return service_->CreateNewRound(
        std::make_shared<Dialog>(MapToPermanentID(dialog->ID()), dialog->Entity()), 
        std::move(question),
        std::move(history_rounds));
}


void UnifiedDialogModel::DeleteRound(DialogID dialog_id, RoundID round_id) {
    service_->DeleteRound(MapToPermanentID(dialog_id), round_id);
}


DialogID UnifiedDialogModel::MapToPermanentID(DialogID dialog_id) const {

    auto transient_id = dialog_id.TransientID();
    if (transient_id) {
        auto permanent_id = zaf::Find(dialog_permanent_id_map_, *transient_id);
        if (permanent_id) {
            return DialogID{ *permanent_id };
        }
    }
    return dialog_id;
}

}