#include "module/ai/gpt/dialog/unified_dialog_view.h"
#include <zaf/base/as.h>
#include "module/ai/gpt/dialog/dialog_view.h"

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(UnifiedDialogView);

UnifiedDialogView::UnifiedDialogView(std::shared_ptr<DialogManager> dialog_manager) :
    dialog_manager_(std::move(dialog_manager)) {

}


void UnifiedDialogView::AfterParse() {

    __super::AfterParse();

    listView->SetDataSource(dialog_manager_->DataSource());

    Subscriptions() += listView->ListControl()->SelectionChangedEvent().Subscribe(
        std::bind_front(&UnifiedDialogView::OnListSelectionChanged, this));
}


void UnifiedDialogView::OnListSelectionChanged(
    const zaf::ListControlSelectionChangedInfo& event_info) {

    auto selected_data = listView->ListControl()->GetFirstSelectedItemData();
    auto dialog = zaf::As<Dialog>(selected_data);
    if (!dialog) {
        return;
    }

    auto dialog_view = zaf::Create<DialogView>(dialog);
    splitControl->SetSecondPane(dialog_view);
    dialog_view->SetFocusToInputEdit();
}


void UnifiedDialogView::StartNewDialog(std::wstring question) {

    auto new_dialog = dialog_manager_->CreateNewDialog();
    new_dialog->CreateRound(std::move(question));
}

}