#include "module/ai/gpt/dialog/round.h"

namespace ra::mod::ai::gpt {

Round::Round(std::uint64_t id, std::wstring question, zaf::Observable<ChatCompletion> answer) :
    id_(id),
    answer_(std::move(answer)) {

}


Round::~Round() {

}


void Round::Remove() {
    remove_event_.AsObserver().OnNext(id_);
}


void Round::Retry() {
    retry_event_.AsObserver().OnNext(shared_from_this());
}

}