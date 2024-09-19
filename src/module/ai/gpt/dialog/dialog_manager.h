#pragma once

#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/dialog_window.h"

namespace ra::mod::ai::gpt {

class DialogManager {
public:
    explicit DialogManager(std::shared_ptr<OpenAIClient> client);

    const std::shared_ptr<DialogWindow>& GetDialogWindow();

private:
    std::shared_ptr<Dialog> dialog_;
    std::shared_ptr<DialogWindow> dialog_window_;
};

}