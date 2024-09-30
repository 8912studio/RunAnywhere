#include "module/ai/gpt/dialog/unified_dialog_view.h"
#include <zaf/base/as.h>

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(UnifiedDialogView);

UnifiedDialogView::UnifiedDialogView(std::shared_ptr<UnifiedDialogModel> model) :
    model_(std::move(model)) {

}


void UnifiedDialogView::AfterParse() {

    __super::AfterParse();

    listView->SetDataSource(model_->DialogDataSource());

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

    auto dialog_view_controller = std::make_unique<DialogModel>(
        dialog, 
        model_);

    current_dialog_view_ = zaf::Create<DialogView>(std::move(dialog_view_controller));
    splitControl->SetSecondPane(current_dialog_view_);
    current_dialog_view_->SetFocusToInputEdit();
}


void UnifiedDialogView::StartNewDialog(std::wstring question) {

    auto new_dialog = model_->CreateNewDialog();
    //new_dialog->CreateRound(std::move(question));
}

}