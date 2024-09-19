#include "module/ai/gpt/gpt_executor.h"
#include "module/ai/gpt/dialog/dialog_window.h"

namespace ra::mod::ai::gpt {

GPTExecutor::GPTExecutor(std::shared_ptr<DialogManager> dialog_manager) :
    dialog_manager_(std::move(dialog_manager)) {

}


void GPTExecutor::SetQuestion(std::wstring question) {
    question_ = std::move(question);
}


ExecuteResult GPTExecutor::Execute() {

    auto new_dialog = dialog_manager_->CreateNewDialog();
    auto dialog_window = zaf::Create<DialogWindow>(new_dialog);
    dialog_window->View()->Chat(std::move(question_));
    dialog_window->Open();
    return PostExecuteAction::Dispose;
}

}