#pragma once

#include <zaf/base/non_copyable.h>
#include <zaf/rx/subject.h>
#include "module/ai/gpt/dialog/round.h"

namespace ra::mod::ai::gpt {

struct RoundPrependedInfo {
    std::size_t count{};
};

struct RoundAppendedInfo {
    std::size_t count{};
};

struct RoundDeletedInfo {
    std::size_t index{};
};

class RoundDataSource : zaf::NonCopyableNonMovable {
public:
    void PrependRounds(RoundList rounds);
    void AppendRounds(RoundList rounds);

    std::shared_ptr<Round> DeleteAndTakeRound(RoundID id);

    std::size_t GetRoundCount() const {
        return rounds_.size();
    }

    std::shared_ptr<Round> GetRoundAtIndex(std::size_t index) const {
        return rounds_[index];
    }

    zaf::Observable<RoundPrependedInfo> RoundPrependedEvent() const {
        return prepended_event_.AsObservable();
    }

    zaf::Observable<RoundAppendedInfo> RoundAppendedEvent() const {
        return appended_event_.AsObservable();
    }

    zaf::Observable<RoundDeletedInfo> RoundDeletedEvent() const {
        return deleted_event_.AsObservable();
    }

private:
    RoundList rounds_;

    zaf::Subject<RoundPrependedInfo> prepended_event_;
    zaf::Subject<RoundAppendedInfo> appended_event_;
    zaf::Subject<RoundDeletedInfo> deleted_event_;
};

}