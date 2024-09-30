#include "module/ai/gpt/dialog/list/dialog_data_source.h"
#include <zaf/base/container/utility/append.h>
#include <zaf/base/container/utility/sort.h>

namespace ra::mod::ai::gpt {
namespace {

bool DialogLessComparer(
    const std::shared_ptr<Dialog>& dialog1,
    const std::shared_ptr<Dialog>& dialog2) {

    auto id1 = dialog1->ID();
    const auto& entity1 = dialog1->Entity();

    auto id2 = dialog2->ID();
    const auto& entity2 = dialog2->Entity();

    return
        std::tie(entity1.update_time, entity1.create_time, id1) >
        std::tie(entity2.update_time, entity2.create_time, id2);
}

}


std::size_t DialogDataSource::GetDataCount() {
    return dialogs_.size();
}


std::shared_ptr<zaf::Object> DialogDataSource::GetDataAtIndex(std::size_t index) {
    return dialogs_[index];
}


void DialogDataSource::AddDialog(std::shared_ptr<Dialog> dialog) {

    auto iterator = std::lower_bound(
        dialogs_.begin(),
        dialogs_.end(),
        dialog,
        DialogLessComparer);

    auto insert_index = std::distance(dialogs_.begin(), iterator);

    dialogs_.insert(iterator, std::move(dialog));

    NotifyDataAdd(insert_index, 1);
}


void DialogDataSource::AddDialogs(std::vector<std::shared_ptr<Dialog>> dialogs) {

    if (dialogs.empty()) {
        return;
    }

    zaf::Sort(dialogs, DialogLessComparer);

    if (dialogs_.empty()) {
        dialogs_ = std::move(dialogs);
        NotifyDataAdd(0, dialogs_.size());
        return;
    }

    if (DialogLessComparer(dialogs_.back(), dialogs.front())) {
        auto insert_index = dialogs_.size();
        zaf::Append(dialogs_, dialogs);
        NotifyDataAdd(insert_index, dialogs.size());
        return;
    }

    for (auto& each_dialog : dialogs) {
        AddDialog(std::move(each_dialog));
    }
}

}