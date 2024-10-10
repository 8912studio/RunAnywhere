#include "module/ai/gpt/dialog/dialog_service.h"
#include <zaf/base/container/utility/find.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/network/response_parsing.h"

namespace ra::mod::ai::gpt {

DialogService::DialogService(
    std::shared_ptr<OpenAIClient> client,
    std::shared_ptr<GPTStorage> storage) 
    :
    client_(std::move(client)),
    storage_(std::move(storage)) {

}


zaf::Observable<DialogList> DialogService::FetchDialogs() {

    return storage_->DialogStorage()->FetchAllDialogs().Map<DialogList>(
        [this](const std::vector<DialogEntity>& dialog_entities) {

        DialogList all_dialogs;

        std::map<DialogPermanentID, std::shared_ptr<Dialog>> permanent_dialogs;
        for (const auto& each_pair : create_round_tasks_) {

            const auto& task = each_pair.second;
            auto permanent_id = task->GetDialogPermanentID();
            if (permanent_id) {
                permanent_dialogs[*permanent_id] = task->GetDialog();
            }
            else {
                all_dialogs.push_back(task->GetDialog());
            }
        }

        for (const auto& each_entity : dialog_entities) {

            DialogPermanentID permanent_id{ each_entity.id };
            auto permanent_dialog = zaf::Find(permanent_dialogs, permanent_id);
            if (permanent_dialog) {
                all_dialogs.push_back(*permanent_dialog);
            }

            DialogID dialog_id{ permanent_id };
            auto dialog = std::make_shared<Dialog>(dialog_id, each_entity);
            all_dialogs.push_back(std::move(dialog));
        }

        return all_dialogs;
    });
}


std::shared_ptr<Dialog> DialogService::CreateNewDialog() {

    DialogEntity entity;
    entity.create_time = std::time(nullptr);
    entity.update_time = entity.create_time;

    DialogID dialog_id{ DialogTransientID{ new_dialog_transient_id_++ } };
    auto new_dialog = std::make_shared<Dialog>(dialog_id, std::move(entity));

    dialog_created_event_.AsObserver().OnNext({ .dialog = new_dialog });
    return new_dialog;
}


zaf::Observable<RoundList> DialogService::FetchRoundsInDialog(DialogID dialog_id) {

    if (dialog_id.TransientID()) {
        auto task = zaf::Find(create_round_tasks_, dialog_id);
        if (task) {
            return zaf::rx::Just(RoundList{ (*task)->GetRound() });
        }
        return zaf::rx::Just(RoundList{});
    }

    auto entities = storage_->RoundStorage()->FetchAllRoundsInDialog(
        dialog_id.PermanentID()->Value());

    return entities.Map<RoundList>([this, dialog_id](const std::vector<RoundEntity>& entities) {

        auto task = zaf::Find(create_round_tasks_, dialog_id);

        RoundList result;
        for (const auto& each_entity : entities) {

            if (task) {
                auto creating_round_permanent_id = (*task)->GetRoundPermanentID();
                if (creating_round_permanent_id && 
                    creating_round_permanent_id->Value() == each_entity.id) {

                    result.push_back((*task)->GetRound());
                    continue;
                }
            }

            auto round_model = std::make_shared<Round>(
                RoundID{ RoundPermanentID{ each_entity.id } },
                zaf::FromUTF8String(each_entity.question),
                CreateRoundAnswerFromEntity(each_entity));

            result.push_back(std::move(round_model));
        }
        return result;
    });
}


zaf::Observable<ChatCompletion> DialogService::CreateRoundAnswerFromEntity(
    const RoundEntity& entity) {

    auto parsed = ParseChatCompletion(entity.response);
    if (parsed) {
        return zaf::rx::Just(*parsed);
    }
    return zaf::rx::Just(ChatCompletion{ Message{ L"" }, std::nullopt });
}


std::shared_ptr<Round> DialogService::CreateNewRound(
    std::shared_ptr<Dialog> dialog,
    std::vector<Message> messages) {

    auto task = std::make_shared<CreateRoundTask>(client_, storage_);
    create_round_tasks_[dialog->ID()] = task;

    Subscriptions() += task->DialogUpdatedEvent().Subscribe(
        [this](const DialogUpdatedInfo& event_info) {
            dialog_updated_event_.AsObserver().OnNext(event_info);
        });

    Subscriptions() += task->RoundCreatedEvent().Subscribe(
        [this](const RoundCreatedInfo& event_info) {
            round_created_event_.AsObserver().OnNext(event_info);
        });

    Subscriptions() += task->FinishEvent().Subscribe(
        [this, dialog](const CreateRoundTaskFinishInfo& event_info) {

            create_round_tasks_.erase(dialog->ID());

            if (event_info.dialog_persisted_info) {
                dialog_persisted_event_.AsObserver().OnNext(*event_info.dialog_persisted_info);
            }

            if (event_info.round_persisted_info) {
                round_persisted_event_.AsObserver().OnNext(*event_info.round_persisted_info);
            }
        });

    task->Run(
        std::move(dialog), 
        RoundTransientID{ new_round_transient_id_++ }, 
        std::move(messages));

    return task->GetRound();
}


void DialogService::DeleteRound(DialogID dialog_id, RoundID round_id) {

    if (TryToDeleteCreatingRound(dialog_id, round_id)) {
        return;
    }

    if (auto permanent_id = round_id.PermanentID()) {

        Subscriptions() += 
            storage_->RoundStorage()->DeleteRound(permanent_id->Value()).Subscribe();
    }
}


bool DialogService::TryToDeleteCreatingRound(DialogID dialog_id, RoundID round_id) {

    auto task = zaf::Find(create_round_tasks_, dialog_id);
    if (!task) {
        return false;
    }

    if ((*task)->GetRound()->ID() != round_id) {
        return false;
    }

    Subscriptions() += (*task)->FinishEvent()
        .FlatMap<std::uint64_t>([this](const CreateRoundTaskFinishInfo& event_info) {
            if (event_info.round_persisted_info) {
                return storage_->RoundStorage()->DeleteRound(
                    event_info.round_persisted_info->permanent_id.Value());
            }
            return zaf::rx::Just<std::uint64_t>(0);
        })
        .Subscribe();

    return true;
}

}