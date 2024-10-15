#include "module/ai/gpt/dialog/post_create_round_task_queue.h"
#include <zaf/base/container/utility/erase.h>
#include <zaf/rx/creation.h>

namespace ra::mod::ai::gpt {
namespace {

struct WrapTaskFinishedInfo {
    std::weak_ptr<PostCreateRoundTask> task;
    RoundTaskFinishedInfo finished_info;
};

}

void PostCreateRoundTaskQueue::AddTask(std::shared_ptr<PostCreateRoundTask> task) {

    tasks_.push_back(std::move(task));

    std::vector<zaf::Observable<WrapTaskFinishedInfo>> all_finished_events;
    for (const auto& each_task : tasks_) {

        std::weak_ptr<PostCreateRoundTask> weak_task = each_task;

        auto wrap_event = each_task->FinishedEvent()
            .Map<WrapTaskFinishedInfo>([weak_task](const RoundTaskFinishedInfo& finished_info) {
                return WrapTaskFinishedInfo{ weak_task, finished_info };
            });

        all_finished_events.push_back(wrap_event);
    }

    concat_finished_sub_ = zaf::rx::Concat<WrapTaskFinishedInfo>(all_finished_events)
        .Do([this](const WrapTaskFinishedInfo& event_info) {
            
            if (!first_finished_info_) {
                first_finished_info_ = event_info.finished_info;
            }
        }, 
        [this]() {

            RoundTaskQueueFinishedInfo event_info;
            if (first_finished_info_) {
                event_info.dialog_persisted_info = first_finished_info_->dialog_persisted_info;
            }

            auto observer = all_finished_event_.AsObserver();
            observer.OnNext(event_info);
            observer.OnCompleted();
        })
        .Subscribe();
}


std::optional<DialogPermanentID> PostCreateRoundTaskQueue::GetDialogPersistedID() const {
    
    if (first_finished_info_) {
        if (first_finished_info_->dialog_persisted_info) {
            return first_finished_info_->dialog_persisted_info->permanent_id;
        }
    }

    return std::nullopt;
}


std::shared_ptr<Dialog> PostCreateRoundTaskQueue::GetLatestDialog() const {

    if (!tasks_.empty()) {
        return tasks_.back()->PreTask()->GetDialog();
        
    }
    return nullptr;
}


RoundList PostCreateRoundTaskQueue::GetAllRounds() const {

    RoundList result;
    for (const auto& each_task : tasks_) {
        result.push_back(each_task->PreTask()->GetRound());
    }
    return result;
}


std::set<RoundPermanentID> PostCreateRoundTaskQueue::GetAllPersistedRoundIDs() const {

    std::set<RoundPermanentID> result;
    for (const auto& each_task : tasks_) {

        auto permanent_id = each_task->GetPersistedRoundID();
        if (permanent_id) {
            result.insert(*permanent_id);
        }
    }
    return result;
}


std::shared_ptr<PostCreateRoundTask> PostCreateRoundTaskQueue::GetTask(RoundID round_id) {

    for (const auto& each_task : tasks_) {
        if (each_task->PreTask()->GetRound()->ID() == round_id) {
            return each_task;
        }
    }
    return nullptr;
}

}