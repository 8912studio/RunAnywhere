#include "module/ai/gpt/dialog/round.h"

namespace ra::mod::ai::gpt {

Round::Round(const RoundID& id, std::wstring question, zaf::Observable<ChatCompletion> answer) :
    id_(id),
    question_(std::move(question)),
    answer_(std::move(answer)) {

}


Round::~Round() {

}

}