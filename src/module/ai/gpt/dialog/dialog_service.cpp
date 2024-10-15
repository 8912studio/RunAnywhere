#include "module/ai/gpt/dialog/dialog_service.h"
#include <zaf/base/container/utility/erase.h>
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
        for (const auto& each_pair : ongoing_round_infos_) {

            auto latest_dialog = each_pair.second->post_task_queue.GetLatestDialog();
            if (!latest_dialog) {
                continue;
            }

            std::optional<DialogPermanentID> permanent_id;
            if (latest_dialog->ID().TransientID()) {
                permanent_id = each_pair.second->post_task_queue.GetDialogPersistedID();
            }
            else {
                permanent_id = *latest_dialog->ID().PermanentID();
            }

            if (permanent_id) {
                permanent_dialogs[*permanent_id] = latest_dialog;
            }
            else {
                all_dialogs.push_back(latest_dialog);
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

    std::vector<zaf::Observable<RoundList>> observables;

    auto ongoing_info = zaf::Find(ongoing_round_infos_, dialog_id);
    if (ongoing_info) {
        auto ongoing_rounds = (*ongoing_info)->post_task_queue.GetAllRounds();
        observables.push_back(zaf::rx::Just(ongoing_rounds));
    }

    if (auto permanent_id = dialog_id.PermanentID()) {
        observables.push_back(FetchRoundsFromStorage(*permanent_id, *ongoing_info));
    }

    return zaf::rx::Concat<RoundList>(observables);
}


zaf::Observable<RoundList> DialogService::FetchRoundsFromStorage(
    DialogPermanentID dialog_id,
    const std::shared_ptr<OngoingRoundInfo>& ongoing_info) {

    auto entities = storage_->RoundStorage()->FetchAllRoundsInDialog(dialog_id.Value());
    return entities.Map<RoundList>(
        [this, dialog_id, ongoing_info](const std::vector<RoundEntity>& entities) {

        std::set<RoundPermanentID> persisted_round_ids;
        if (ongoing_info) {
            persisted_round_ids = ongoing_info->post_task_queue.GetAllPersistedRoundIDs();
        }

        RoundList result;
        for (const auto& each_entity : entities) {

            //Ignore the round which is ongoing.
            if (persisted_round_ids.contains(RoundPermanentID{ each_entity.id })) {
                continue;
            }

            auto round = CreateRoundFromEntity(each_entity);
            result.push_back(std::move(round));
        }
        return result;
    });
}


std::shared_ptr<Round> DialogService::CreateRoundFromEntity(const RoundEntity& entity) {

    RoundID round_id{ RoundPermanentID{ entity.id } };
    auto question = zaf::FromUTF8String(entity.question);

    //No response, create a round in pending state.
    if (entity.response.empty()) {
        return std::make_shared<Round>(round_id, entity.update_time, std::move(question));
    }

    //Create a round in completed state.
    auto parsed = ParseChatCompletion(entity.response);
    if (parsed) {
        return std::make_shared<Round>(
            round_id, 
            entity.update_time, 
            std::move(question), 
            std::move(*parsed));
    }

    //Bad response, create a round in pending state.
    return std::make_shared<Round>(round_id, entity.update_time, std::move(question));
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
    std::wstring question,
    RoundList history_rounds) {

    auto ongoing_info = GetOngoingRoundInfo(dialog->ID());
    bool has_ongoing_pre_task = !ongoing_info->pre_tasks.empty();

    auto pre_task = CreatePreCreateRoundTask(dialog);
    ongoing_info->pre_tasks.push_back(pre_task);

    auto post_task = std::make_shared<PostCreateRoundTask>(storage_, pre_task);
    //The post task should be add to the queue before subscribing to its finished event.
    ongoing_info->post_task_queue.AddTask(post_task);

    Subscriptions() += post_task->FinishedEvent().Subscribe(
        [this](const RoundTaskFinishedInfo& event_info) {

        if (event_info.round_persisted_info) {
            round_persisted_event_.AsObserver().OnNext(*event_info.round_persisted_info);
        }
    });

    pre_task->Run(CreateRoundParameters{
        .dialog = std::move(dialog),
        .round_transient_id = RoundTransientID{ new_round_transient_id_++ },
        .question = std::move(question),
        .history_rounds = history_rounds,
        .has_ongoing_pre_task = has_ongoing_pre_task,
    });

    post_task->Run();

    return pre_task->GetRound();
}


std::shared_ptr<PreCreateRoundTask> DialogService::CreatePreCreateRoundTask(
    const std::shared_ptr<Dialog>& dialog) {

    auto pre_task = std::make_shared<PreCreateRoundTask>(client_);

    Subscriptions() += pre_task->DialogUpdatedEvent().Subscribe(
        [this](const DialogUpdatedInfo& event_info) {
            dialog_updated_event_.AsObserver().OnNext(event_info);
        });

    Subscriptions() += pre_task->RoundCreatedEvent().Subscribe(
        [this](const RoundCreatedInfo& event_info) {
            round_created_event_.AsObserver().OnNext(event_info);
        });

    Subscriptions() += pre_task->TaskFinishedEvent()
        .DoOnTerminated([this, dialog, weak_task = std::weak_ptr{ pre_task }]() {

            auto ongoing_info = zaf::Find(ongoing_round_infos_, dialog->ID());
            if (!ongoing_info) {
                return;
            }

            auto pre_task = weak_task.lock();
            if (!pre_task) {
                return;
            }

            zaf::EraseIf((*ongoing_info)->pre_tasks, [&pre_task](const auto& task) {
                return pre_task == task;
            });
        })
        .Subscribe();

    return pre_task;
}


DialogService::OngoingRoundInfo* DialogService::GetOngoingRoundInfo(DialogID dialog_id) {

    auto& ongoing_info = ongoing_round_infos_[dialog_id];
    if (!ongoing_info) {

        ongoing_info = std::make_unique<OngoingRoundInfo>();

        Subscriptions() += ongoing_info->post_task_queue.AllFinishedEvent().Subscribe(
            [this, dialog_id](const RoundTaskQueueFinishedInfo& event_info) {

            ongoing_round_infos_.erase(dialog_id);

            if (event_info.dialog_persisted_info) {
                dialog_persisted_event_.AsObserver().OnNext(*event_info.dialog_persisted_info);
            }
        });
    }

    return ongoing_info.get();
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

    auto ongoing_info = zaf::Find(ongoing_round_infos_, dialog_id);
    if (!ongoing_info) {
        return false;
    }

    auto task = (*ongoing_info)->post_task_queue.GetTask(round_id);
    if (!task) {
        return false;
    }

    Subscriptions() += task->FinishedEvent()
        .FlatMap<std::uint64_t>([this](const RoundTaskFinishedInfo& event_info) {
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