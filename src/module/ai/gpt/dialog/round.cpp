#include "module/ai/gpt/dialog/round.h"

namespace ra::mod::ai::gpt {

Round::Round(const RoundID& id, std::time_t updated_time, std::wstring question) :
    id_(id), 
    updated_time_(updated_time),
    question_(std::move(question)),
    state_(RoundState::Pending) {

}


Round::Round(
    const RoundID& id,
    std::time_t updated_time, 
    std::wstring question,
    ChatCompletion answer) 
    :
    id_(id),
    updated_time_(updated_time),
    question_(std::move(question)),
    result_(std::move(answer)),
    state_(RoundState::Completed) {

}


Round::Round(
    const RoundID& id,
    std::time_t updated_time,
    std::wstring question,
    zaf::Observable<ChatCompletion> answer) 
    :
    id_(id),
    updated_time_(updated_time),
    question_(std::move(question)),
    state_(RoundState::Ongoing) {

    Subscriptions() += answer.Do(
        [this](const ChatCompletion& completion) {
            result_ = completion;
            state_ = RoundState::Completed;
        }, 
        [this](std::exception_ptr error) {
            result_ = error;
            state_ = RoundState::Error;
        })
        .DoOnTerminated([this]() {
            state_changed_event_.AsObserver().OnNext(state_);
            state_changed_event_.AsObserver().OnCompleted();
        })
        .Subscribe();
}


Round::~Round() {

}

}