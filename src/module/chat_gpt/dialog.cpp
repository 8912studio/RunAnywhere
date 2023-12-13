#include "module/chat_gpt/dialog.h"
#include <zaf/base/error/check.h>
#include <zaf/rx/creation.h>
#include "module/chat_gpt/local_error.h"

namespace ra::mod::chat_gpt {

Dialog::Dialog(std::shared_ptr<comm::OpenAIClient> client) : client_(std::move(client)) {

}


zaf::Observable<comm::ChatCompletion> Dialog::Chat(std::wstring question_content) {

    if (ongoing_question_.has_value()) {
        return zaf::rx::Throw<comm::ChatCompletion>(zaf::Error{ 
            make_error_code(LocalErrc::ChatOngoing) 
        });
    }

    ongoing_question_.emplace(std::move(question_content));

    std::vector<const Message*> sent_messages;
    sent_messages.reserve(history_rounds_.size() + 1);

    for (const auto& each_round : history_rounds_) {
        sent_messages.push_back(&each_round.question);
        sent_messages.push_back(&each_round.answer);
    }
    sent_messages.push_back(&*ongoing_question_);

    return client_->CreateChatCompletion(sent_messages)
        .Do([this](const comm::ChatCompletion& result) {

            history_rounds_.emplace_back(std::move(*ongoing_question_), result.Message());

            if (history_rounds_.size() > max_history_rounds_count_) {
                history_rounds_.pop_front();
            }
        })
        .Finally([this]() {
            ongoing_question_.reset();
        });
}

}