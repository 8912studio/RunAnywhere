#include "module/ai/gpt/dialog/content/round_model.h"

namespace ra::mod::ai::gpt {

RoundModel::RoundModel(
    const RoundID& id,
    std::wstring question, 
    zaf::Observable<ChatCompletion> answer)
    :
    id_(id),
    question_(std::move(question)),
    answer_(std::move(answer)) {

}

}