#pragma once

#include <map>
#include <optional>
#include <vector>
#include <zaf/control/list_data_source.h>
#include "module/ai/gpt/dialog/dialog.h"

namespace ra::mod::ai::gpt {

class DialogDataSource : public zaf::ListDataSource {
public:
    std::size_t GetDataCount() const override;
    std::shared_ptr<zaf::Object> GetDataAtIndex(std::size_t index) const override;

    void AddDialog(std::shared_ptr<Dialog> dialog);
    void AddDialogs(std::vector<std::shared_ptr<Dialog>> dialogs);

    void UpdateDialog(std::shared_ptr<Dialog> dialog);

    std::optional<std::size_t> GetIndexOfDialog(DialogID dialog_id) const;
    std::shared_ptr<Dialog> GetDialogAtIndex(std::size_t index) const;

private:
    void InsertDialogs(
        std::size_t insert_index, 
        DialogList::const_iterator begin, 
        DialogList::const_iterator end);

private:
    std::vector<std::shared_ptr<Dialog>> dialogs_;
    std::map<DialogID, std::shared_ptr<Dialog>> dialog_map_;
};

}