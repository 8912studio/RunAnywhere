#pragma once

#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/dialog_data_source.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

class DialogManager : zaf::SubscriptionHost {
public:
    DialogManager(
        std::shared_ptr<OpenAIClient> client,
        std::shared_ptr<GPTStorage> storage);

    void Initialize();

    const std::shared_ptr<DialogDataSource>& DataSource() const {
        return data_source_;
    }

    std::shared_ptr<Dialog> CreateNewDialog();

private:
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<GPTStorage> storage_;

    std::size_t new_dialog_transient_id_{ 1 };
    std::shared_ptr<DialogDataSource> data_source_;
};

}