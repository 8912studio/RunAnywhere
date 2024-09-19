#pragma once

#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/dialog.h"

namespace ra::mod::ai::gpt {

class DialogManager {
public:
    explicit DialogManager(std::shared_ptr<OpenAIClient> client);

    std::shared_ptr<Dialog> CreateNewDialog();

private:
    std::size_t new_dialog_number_{ 1 };
    std::shared_ptr<OpenAIClient> client_;
};

}