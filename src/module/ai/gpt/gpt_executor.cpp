#include "module/ai/gpt/gpt_executor.h"

namespace ra::mod::ai::gpt {

GPTExecutor::GPTExecutor(std::shared_ptr<DialogManager> dialog_manager) :
    dialog_manager_(std::move(dialog_manager)) {

}


void GPTExecutor::SetQuestion(std::wstring question) {
    question_ = std::move(question);
}


ExecuteResult GPTExecutor::Execute() {

    const auto& dialog_window = dialog_manager_->GetDialogWindow();
    dialog_window->Chat(std::move(question_));
    dialog_window->Open();
    return PostExecuteAction::Dispose;
}

}