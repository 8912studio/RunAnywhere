#pragma once

#include <zaf/base/none.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/gpt_module_context.h"
#include "module/command_executor.h"

namespace ra::mod::ai::gpt {

class GPTExecutor : public CommandExecutor, zaf::SubscriptionHost {
public:
    explicit GPTExecutor(std::shared_ptr<GPTModuleContext>);

    void SetQuestion(std::wstring question);

    ExecuteResult Execute() override;

private:
    std::shared_ptr<GPTModuleContext> context_;
    std::wstring question_;
};

}