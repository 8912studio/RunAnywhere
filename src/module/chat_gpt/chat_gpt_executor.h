#pragma once

#include <zaf/base/none.h>
#include <zaf/rx/subscription_host.h>
#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/command_executor.h"

namespace ra::mod::chat_gpt {

class ChatGPTExecutor : public CommandExecutor, zaf::SubscriptionHost {
public:
    explicit ChatGPTExecutor(std::shared_ptr<comm::OpenAIClient>);

    zaf::Observable<zaf::None> BeginEvent() const {
        return begin_event_.AsObservable();
    }

    zaf::Observable<comm::ChatCompletion> FinishEvent() const {
        return finish_event_.AsObservable();
    }

    void SetQuestion(std::wstring question);

    ExecuteResult Execute() override;

private:
    void InnerExecute();

private:
    std::shared_ptr<comm::OpenAIClient> client_;
    bool has_executed_{};
    std::wstring question_;
    zaf::Subject<zaf::None> begin_event_;
    zaf::Subject<comm::ChatCompletion> finish_event_;
};

}