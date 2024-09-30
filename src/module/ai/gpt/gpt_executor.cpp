#include "module/ai/gpt/gpt_executor.h"
#include "module/ai/gpt/dialog/unified_dialog_model.h"

namespace ra::mod::ai::gpt {

GPTExecutor::GPTExecutor(std::shared_ptr<GPTModuleContext> context) :
    context_(std::move(context)) {

}


void GPTExecutor::SetQuestion(std::wstring question) {
    question_ = std::move(question);
}


ExecuteResult GPTExecutor::Execute() {

    auto unified_dialog_window = context_->UnifiedDialogWindow();
    if (!unified_dialog_window) {

        auto model = std::make_shared<UnifiedDialogModel>(context_->DialogService());
        model->Initialize();

        unified_dialog_window = zaf::Create<UnifiedDialogWindow>(std::move(model));
        context_->SetUnifiedDialogWindow(unified_dialog_window);
    }

    unified_dialog_window->Show();
    unified_dialog_window->View()->StartNewDialog(std::move(question_));
    return PostExecuteAction::Dispose;
}

}