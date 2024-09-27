#include "module/ai/gpt/gpt_executor.h"

namespace ra::mod::ai::gpt {

GPTExecutor::GPTExecutor(std::shared_ptr<GPTModuleContext> context) :
    context_(std::move(context)) {

}


void GPTExecutor::SetQuestion(std::wstring question) {
    question_ = std::move(question);
}


ExecuteResult GPTExecutor::Execute() {

    auto unified_dialog_window = context_->UnifiedDialogWindow();
    unified_dialog_window->Show();
    unified_dialog_window->View()->StartNewDialog(std::move(question_));
    return PostExecuteAction::Dispose;
}

}