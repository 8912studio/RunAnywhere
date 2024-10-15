#pragma once

#include <deque>
#include <zaf/base/non_copyable.h>
#include "module/ai/gpt/dialog/post_create_round_task.h"

namespace ra::mod::ai::gpt {

struct RoundTaskQueueFinishedInfo {
    std::optional<DialogPersistedInfo> dialog_persisted_info;
};

class PostCreateRoundTaskQueue : zaf::NonCopyableNonMovable {
public:
    void AddTask(std::shared_ptr<PostCreateRoundTask> task);

    std::optional<DialogPermanentID> GetDialogPersistedID() const;
    std::shared_ptr<Dialog> GetLatestDialog() const;

    RoundList GetAllRounds() const;
    std::set<RoundPermanentID> GetAllPersistedRoundIDs() const;

    std::shared_ptr<PostCreateRoundTask> GetTask(RoundID round_id);

    zaf::Observable<RoundTaskQueueFinishedInfo> AllFinishedEvent() const {
        return all_finished_event_.AsObservable();
    }

private:
    std::deque<std::shared_ptr<PostCreateRoundTask>> tasks_;
    zaf::Subscription concat_finished_sub_;
    std::optional<RoundTaskFinishedInfo> first_finished_info_;
    zaf::Subject<RoundTaskQueueFinishedInfo> all_finished_event_;
};

}