#include "module/ai/gpt/storage/dialog_storage.h"
#include "utility/sql/table_schema.h"

using namespace ra::utility::sql;

namespace ra::mod::ai::gpt {

DialogStorage::DialogStorage(std::shared_ptr<ScheduledStorageContext> context) :
    context_(std::move(context)) {

}


zaf::Observable<std::vector<DialogEntity>> DialogStorage::FetchAllDialogs() {

    return context_->Execute<std::vector<DialogEntity>>([this](StorageContext& context) {
        return context.DialogDataContext().SelectAll();
    });
}


zaf::Observable<std::uint64_t> DialogStorage::AddDialog(const DialogEntity& dialog) {

    return context_->Execute<std::uint64_t>([this, dialog](StorageContext& context) {
        return context.DialogDataContext().AutoIncInsert(dialog);
    });
}


zaf::Observable<std::uint64_t> DialogStorage::UpdateDialog(const DialogEntity& dialog) {

    return context_->Execute<std::uint64_t>([this, dialog](StorageContext& context) {
        context.DialogDataContext().Update(dialog);
        return dialog.id;
    });
}


zaf::Observable<zaf::None> DialogStorage::DeleteDialog(std::uint64_t dialog_id) {

    return context_->Execute<zaf::None>([this, dialog_id](StorageContext& context) {
        context.DialogDataContext().Delete(dialog_id);
        return zaf::None{};
    });
}

}