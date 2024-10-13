#include "module/ai/gpt/dialog/content/round_data_source.h"
#include <zaf/base/container/utility/sort.h>

namespace ra::mod::ai::gpt {
namespace {

bool RoundLessComparer(
    const std::shared_ptr<Round>& round1, 
    const std::shared_ptr<Round>& round2) {

    return round1->ID() < round2->ID();
}

}

void RoundDataSource::PrependRounds(RoundList rounds) {

    zaf::Sort(rounds, RoundLessComparer);

    rounds_.insert(rounds_.begin(), rounds.begin(), rounds.end());

    prepended_event_.AsObserver().OnNext(RoundPrependedInfo{ 
        .count = rounds.size()
    });
}


void RoundDataSource::AppendRounds(RoundList rounds) {

    zaf::Sort(rounds, RoundLessComparer);

    rounds_.insert(rounds_.end(), rounds.begin(), rounds.end());

    appended_event_.AsObserver().OnNext(RoundAppendedInfo{ 
        .count = rounds.size() 
    });
}


void RoundDataSource::DeleteRound(RoundID id) {

    auto iterator = std::lower_bound(
        rounds_.begin(), 
        rounds_.end(), 
        id, 
        [](const auto& round, RoundID id) {
            return round->ID() < id;
        });

    if (iterator == rounds_.end() || (*iterator)->ID() != id) {
        return;
    }

    std::size_t deleted_index = std::distance(rounds_.begin(), iterator);
    rounds_.erase(iterator);

    deleted_event_.AsObserver().OnNext(RoundDeletedInfo{ .index = deleted_index });
}

}