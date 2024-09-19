#include "module/ai/gpt/dialog/dialog_manager.h"
#include <zaf/creation.h>

namespace ra::mod::ai::gpt {

DialogManager::DialogManager(std::shared_ptr<OpenAIClient> client) :
    dialog_(std::make_shared<Dialog>(std::move(client))) {

}


const std::shared_ptr<DialogWindow>& DialogManager::GetDialogWindow() {

    if (!dialog_window_) {
        dialog_window_ = zaf::Create<DialogWindow>(dialog_);
    }

    return dialog_window_;
}

}