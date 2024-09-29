#pragma once

#include <vector>
#include <zaf/control/list_data_source.h>
#include "module/ai/gpt/dialog/list/dialog_item_data.h"

namespace ra::mod::ai::gpt {

class DialogDataSource : public zaf::ListDataSource {
public:
    std::size_t GetDataCount() override;
    std::shared_ptr<zaf::Object> GetDataAtIndex(std::size_t index) override;

    void AddDialog(std::shared_ptr<DialogItemData> dialog);
    void AddDialogs(std::vector<std::shared_ptr<DialogItemData>> dialogs);

private:
    std::vector<std::shared_ptr<DialogItemData>> dialogs_;
};

}