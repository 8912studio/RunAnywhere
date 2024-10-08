#include "module/ai/gpt/dialog/list/dialog_data_source.h"
#include <zaf/base/container/utility/append.h>
#include <zaf/base/container/utility/find.h>
#include <zaf/base/container/utility/sort.h>
#include <zaf/base/error/contract_error.h>
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


std::size_t DialogDataSource::GetDataCount() const {
    return dialogs_.size();
}


std::shared_ptr<zaf::Object> DialogDataSource::GetDataAtIndex(std::size_t index) const {
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
        InsertDialogs(0, new_dialogs.begin(), new_dialogs.end());
        return;
    }

    std::size_t old_current{};
    auto new_range_begin = new_dialogs.begin();
    auto new_range_end = new_dialogs.begin();

    const auto commit_adding = [this, &old_current, &new_range_begin, &new_range_end]() {
        if (new_range_begin != new_range_end) {
            InsertDialogs(old_current, new_range_begin, new_range_end);
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
        else {
            //Not allow to add identical dialog.
            ZAF_EXPECT(old_dialog->ID() != new_dialog->ID());
            ++new_range_end;
        }
    }

    if (new_range_begin != new_dialogs.end()) {
        InsertDialogs(old_current, new_range_begin, new_dialogs.end());
    }
}


void DialogDataSource::InsertDialogs(
    std::size_t insert_index, 
    DialogList::const_iterator begin,
    DialogList::const_iterator end) {

    dialogs_.insert(std::next(dialogs_.begin(), insert_index), begin, end);

    for (auto iterator = begin; iterator != end; ++iterator) {
        dialog_map_[(*iterator)->ID()] = *iterator;
    }

    NotifyDataAdded(insert_index, std::distance(begin, end));
}


void DialogDataSource::UpdateDialog(std::shared_ptr<Dialog> dialog) {

    auto old_dialog = zaf::Find(dialog_map_, dialog->ID());
    if (!old_dialog) {
        return;
    }

    auto old_iterator = std::lower_bound(
        dialogs_.begin(),
        dialogs_.end(),
        *old_dialog,
        DialogLessComparer);

    if (old_iterator == dialogs_.end() || (*old_iterator)->ID() != (*old_dialog)->ID()) {
        return;
    }

    auto old_index = std::distance(dialogs_.begin(), old_iterator);
    dialogs_.erase(old_iterator);

    auto new_iterator = std::lower_bound(
        dialogs_.begin(),
        dialogs_.end(),
        dialog,
        DialogLessComparer);

    auto new_index = std::distance(dialogs_.begin(), new_iterator);
    dialogs_.insert(new_iterator, dialog);
    dialog_map_[dialog->ID()] = dialog;

    NotifyDataMoved(old_index, new_index);
}


std::optional<std::size_t> DialogDataSource::GetIndexOfDialog(DialogID dialog_id) const {
    
    auto dialog = zaf::Find(dialog_map_, dialog_id);
    if (!dialog) {
        return std::nullopt;
    }

    auto iterator = std::lower_bound(
        dialogs_.begin(),
        dialogs_.end(),
        *dialog,
        DialogLessComparer);

    if (iterator == dialogs_.end()) {
        return std::nullopt;
    }

    if ((*iterator)->ID() != (*dialog)->ID()) {
        return std::nullopt;
    }

    return std::distance(dialogs_.begin(), iterator);
}


std::shared_ptr<Dialog> DialogDataSource::GetDialogAtIndex(std::size_t index) const {
    return dialogs_[index];
}

}