#pragma once

#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/chat_gpt/dialog.h"
#include "module/chat_gpt/dialog/dialog_window.h"

namespace ra::mod::chat_gpt {

class DialogManager {
public:
    explicit DialogManager(std::shared_ptr<comm::OpenAIClient> client);

    const std::shared_ptr<DialogWindow>& GetDialogWindow();

private:
    std::shared_ptr<Dialog> dialog_;
    std::shared_ptr<DialogWindow> dialog_window_;
};

}