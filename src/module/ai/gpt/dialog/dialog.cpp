#include "module/ai/gpt/dialog/dialog.h"
#include <format>
#include <zaf/base/container/utility/erase.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/local_error.h"

namespace ra::mod::ai::gpt {

Dialog::Dialog(std::size_t number, std::shared_ptr<OpenAIClient> client) : 
    number_(number),
    client_(std::move(client)) {

}


std::wstring Dialog::Subject() const {

    if (!subject_.empty()) {
        return subject_;
    }

    return std::format(L"New dialog #{}", number_);
}


zaf::Observable<zaf::None> Dialog::SubjectUpdatedEvent() const {
    return subject_updated_event_.GetObservable();
}


std::wstring Dialog::GenerateSubject(const std::wstring& question) {

    constexpr std::size_t max_length = 100;

    if (question.length() <= max_length) {
        return question;
    }

    return std::format(L"{}...", question.substr(0, max_length));
}


std::shared_ptr<Round> Dialog::CreateRound(std::wstring question) {

    auto round_id = ++round_id_seed_;

    auto answer = Chat(round_id, question).Map<std::wstring>(
        [](const ChatCompletion& chat_completion) {
    
        return chat_completion.Message().Content();
    });

    auto round = std::make_shared<Round>(round_id, std::move(question), std::move(answer));
    Subscriptions() += round->RemoveEvent().Subscribe(
        std::bind(&Dialog::OnRoundRemoved, this, std::placeholders::_1));

    return round;
}


zaf::Observable<ChatCompletion> Dialog::Chat(std::uint64_t round_id, std::wstring question) {

    if (ongoing_question_.has_value()) {
        return zaf::rx::Throw<ChatCompletion>(LocalError{ 
            LocalErrorCode::ChatOngoing,
            ZAF_SOURCE_LOCATION(),
        });
    }

    //Generate the subject if there is no subject yet.
    if (subject_.empty()) {
        subject_ = GenerateSubject(question);
        subject_updated_event_.Raise({});
    }

    ongoing_question_.emplace(std::move(question));

    std::vector<const Message*> sent_messages;
    sent_messages.reserve(history_rounds_.size() + 1);

    for (const auto& each_round : history_rounds_) {
        sent_messages.push_back(&each_round.question);
        sent_messages.push_back(&each_round.answer);
    }
    sent_messages.push_back(&*ongoing_question_);

    zaf::ReplaySubject<ChatCompletion> result;

    Subscriptions() += client_->CreateChatCompletion(sent_messages)
        .Do([this, round_id](const ChatCompletion& result) {

            history_rounds_.emplace_back(
                round_id, 
                std::move(*ongoing_question_), 
                result.Message());

            if (history_rounds_.size() > max_history_rounds_count_) {
                history_rounds_.pop_front();
            }
        })
        .Finally([this]() {
            ongoing_question_.reset();
        })
        .Subscribe(result.AsObserver());

    return result.AsObservable();
}


void Dialog::OnRoundRemoved(std::uint64_t round_id) {

    zaf::EraseIf(history_rounds_, [round_id](const RoundData& round) {
        return round.id == round_id;
    });
}

}