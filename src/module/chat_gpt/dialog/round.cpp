#include "module/chat_gpt/dialog/round.h"

namespace ra::mod::chat_gpt {

Round::Round(std::uint64_t id, std::wstring question, zaf::Observable<std::wstring> answer) :
    id_(id),
    question_(std::move(question)),
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