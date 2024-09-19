#pragma once

#include <zaf/base/none.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/dialog_manager.h"
#include "module/command_executor.h"

namespace ra::mod::ai::gpt {

class GPTExecutor : public CommandExecutor, zaf::SubscriptionHost {
public:
    explicit GPTExecutor(std::shared_ptr<DialogManager>);

    void SetQuestion(std::wstring question);

    ExecuteResult Execute() override;

private:
    std::shared_ptr<DialogManager> dialog_manager_;
    std::wstring question_;
};

}