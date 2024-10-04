#pragma once

#include <optional>
#include <vector>
#include <zaf/control/list_data_source.h>
#include "module/ai/gpt/dialog/dialog.h"

namespace ra::mod::ai::gpt {

class DialogDataSource : public zaf::ListDataSource {
public:
    std::size_t GetDataCount() override;
    std::shared_ptr<zaf::Object> GetDataAtIndex(std::size_t index) override;

    void AddDialog(std::shared_ptr<Dialog> dialog);
    void AddDialogs(std::vector<std::shared_ptr<Dialog>> dialogs);

    std::optional<std::size_t> GetIndexOfDialog(const std::shared_ptr<Dialog>& dialog) const;
    std::shared_ptr<Dialog> GetDialogAtIndex(std::size_t index) const;

private:
    std::vector<std::shared_ptr<Dialog>> dialogs_;
};

}