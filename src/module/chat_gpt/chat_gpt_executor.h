#pragma once

#include <zaf/rx/subscription_host.h>
#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/command_executor.h"

namespace ra::mod::chat_gpt {

class ChatGPTExecutor : public CommandExecutor, zaf::SubscriptionHost {
public:
    explicit ChatGPTExecutor(std::shared_ptr<comm::OpenAIClient>);

    zaf::Observable<comm::ChatCompletion> FinishEvent() const {
        return finish_event_.AsObservable();
    }

    void SetQuestion(std::wstring question);

    ExecuteResult Execute() override;

private:
    std::shared_ptr<comm::OpenAIClient> client_;
    std::wstring question_;
    zaf::Subject<comm::ChatCompletion> finish_event_;
};

}