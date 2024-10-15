#pragma once

#include <memory>
#include <string>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/id.h"
#include "module/ai/gpt/network/chat_completion.h"
#include "module/ai/gpt/storage/round_entity.h"

namespace ra::mod::ai::gpt {

enum class RoundState {

    //The question hasn't been sent.
    Pending,

    //The question has been sent, and the round is waiting for the answer.
    Ongoing,

    //There is an error during waiting for the answer.
    Error,

    //Answer is received, the round is completed.
    Completed,
};

class Round : zaf::SubscriptionHost, zaf::NonCopyableNonMovable {
public:
    //Constructs a round in Pending state.
    Round(const RoundID& id, std::time_t updated_time, std::wstring question);

    //Constructs a round in Completed state.
    Round(
        const RoundID& id,
        std::time_t updated_time, 
        std::wstring question, 
        ChatCompletion answer);

    //Constructs a round in Ongoing state.
    Round(
        const RoundID& id, 
        std::time_t updated_time, 
        std::wstring question, 
        zaf::Observable<ChatCompletion> answer);

    ~Round();

    const RoundID& ID() const {
        return id_;
    }

    std::time_t UpdatedTime() const {
        return updated_time_;
    }

    const std::wstring& Question() const {
        return question_;
    }

    RoundState State() const {
        return state_;
    }

    const ChatCompletion& Answer() const {
        return std::get<ChatCompletion>(result_);
    }

    std::exception_ptr Error() const {
        return std::get<std::exception_ptr>(result_);
    }

    zaf::Observable<RoundState> StateChangedEvent() const {
        return state_changed_event_.AsObservable();
    }

private:
    RoundID id_;
    std::time_t updated_time_{};
    std::wstring question_;
    RoundState state_{ RoundState::Pending };
    zaf::Subject<RoundState> state_changed_event_;
    std::variant<std::monostate, ChatCompletion, std::exception_ptr> result_;
};

using RoundList = std::vector<std::shared_ptr<Round>>;

}
