#include "module/ai/gpt/dialog/pre_create_round_task.h"
#include <zaf/base/error/contract_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include "module/ai/gpt/local_error.h"

namespace ra::mod::ai::gpt {

PreCreateRoundTask::PreCreateRoundTask(std::shared_ptr<OpenAIClient> client) : 
    client_(std::move(client)) {

}


void PreCreateRoundTask::Run(const CreateRoundParameters& parameters) {

    ZAF_EXPECT(parameters.dialog);
    ZAF_EXPECT(parameters.round_transient_id.Value() != 0);
    ZAF_EXPECT(!parameters.question.empty());

    auto current_time = std::time(nullptr);
    UpdateDialog(parameters.dialog, current_time, parameters.question);
    CreateRound(parameters.round_transient_id, current_time, parameters.question);
    CreateChat(parameters.question, parameters.history_rounds, parameters.has_ongoing_pre_task);
}


void PreCreateRoundTask::UpdateDialog(
    const std::shared_ptr<Dialog>& dialog,
    std::time_t update_time,
    const std::wstring& question) {

    DialogEntity dialog_entity{ dialog->Entity() };
    dialog_entity.update_time = update_time;

    if (dialog_entity.create_time == 0) {
        dialog_entity.create_time = update_time;
    }

    if (dialog_entity.subject.empty()) {
        dialog_entity.subject = zaf::ToUTF8String(question.substr(0, 100));
    }

    dialog_ = std::make_shared<Dialog>(dialog->ID(), std::move(dialog_entity));
    dialog_updated_event_.AsObserver().OnNext(DialogUpdatedInfo{ .dialog = dialog_ });
    dialog_updated_event_.AsObserver().OnCompleted();
}


void PreCreateRoundTask::CreateRound(
    RoundTransientID round_transient_id, 
    std::time_t update_time, 
    const std::wstring& question) {

    round_ = std::make_shared<Round>(
        RoundID{ round_transient_id },
        update_time,
        question,
        round_finished_event_.AsObservable());

    round_created_event_.AsObserver().OnNext(RoundCreatedInfo{
        .dialog_id = dialog_->ID(),
        .round = round_
    });
    round_created_event_.AsObserver().OnCompleted();
}


void PreCreateRoundTask::CreateChat(
    const std::wstring& question, 
    const RoundList& history_rounds,
    bool has_ongoing_pre_task) {

    if (has_ongoing_pre_task) {
        LocalError local_error(LocalErrorCode::ChatOngoing, ZAF_SOURCE_LOCATION());
        task_finished_event_.AsObserver().OnError(local_error);
        round_finished_event_.AsObserver().OnError(local_error);
        return;
    }

    std::vector<Message> messages;
    for (const auto& each_round : history_rounds) {
        ZAF_EXPECT(each_round->State() == RoundState::Completed);
        messages.push_back(Message{ each_round->Question() });
        messages.push_back(each_round->Answer().Message());
    }

    messages.push_back(Message{ question });

    Subscriptions() += client_->CreateChatCompletion(messages)
        .Do(task_finished_event_.AsObserver())
        .Map<ChatCompletion>([](const ChatResult& chat_result) {
            return chat_result.ChatCompletion();
        })
        .Do(round_finished_event_.AsObserver())
        .Subscribe();
}

}