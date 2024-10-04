#include "module/ai/gpt/dialog/list/dialog_data_source.h"
#include <zaf/base/container/utility/append.h>
#include <zaf/base/container/utility/sort.h>
#include <zaf/base/range.h>

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
    AddDialogs({ std::move(dialog) });
}


void DialogDataSource::AddDialogs(std::vector<std::shared_ptr<Dialog>> new_dialogs) {

    if (new_dialogs.empty()) {
        return;
    }

    zaf::Sort(new_dialogs, DialogLessComparer);

    if (dialogs_.empty()) {
        dialogs_ = std::move(new_dialogs);
        NotifyDataAdd(0, dialogs_.size());
        return;
    }

    std::size_t old_current{};
    auto new_range_begin = new_dialogs.begin();
    auto new_range_end = new_dialogs.begin();

    const auto commit_adding = [this, &old_current, &new_range_begin, &new_range_end]() {
        if (new_range_begin != new_range_end) {

            dialogs_.insert(
                std::next(dialogs_.begin(), old_current),
                new_range_begin,
                new_range_end);

            NotifyDataAdd(old_current, std::distance(new_range_begin, new_range_end));

            old_current += std::distance(new_range_begin, new_range_end);
            new_range_begin = new_range_end;
        }
    };

    while (old_current != dialogs_.size() && new_range_end != new_dialogs.end()) {

        const auto& old_dialog = dialogs_[old_current];
        const auto& new_dialog = *new_range_end;

        if (DialogLessComparer(old_dialog, new_dialog)) {
            commit_adding();
            ++old_current;
        }
        else if (old_dialog->ID() == new_dialog->ID()) {
            commit_adding();

            auto& updated_dialog = dialogs_[old_current];
            updated_dialog = new_dialog;
            NotifyDataUpdate(old_current, 1);

            ++old_current;
            ++new_range_end;
            new_range_begin = new_range_end;
        }
        else {
            ++new_range_end;
        }
    }

    if (new_range_begin != new_dialogs.end()) {

        dialogs_.insert(
            std::next(dialogs_.begin(), old_current),
            new_range_begin,
            new_dialogs.end());

        NotifyDataAdd(old_current, std::distance(new_range_begin, new_dialogs.end()));
    }
}


std::optional<std::size_t> DialogDataSource::GetIndexOfDialog(
    const std::shared_ptr<Dialog>& dialog) const {
    
    auto iterator = std::lower_bound(
        dialogs_.begin(),
        dialogs_.end(),
        dialog,
        DialogLessComparer);

    if (iterator == dialogs_.end()) {
        return std::nullopt;
    }

    if ((*iterator)->ID() != dialog->ID()) {
        return std::nullopt;
    }

    return std::distance(dialogs_.begin(), iterator);
}


std::shared_ptr<Dialog> DialogDataSource::GetDialogAtIndex(std::size_t index) const {
    return dialogs_[index];
}

}