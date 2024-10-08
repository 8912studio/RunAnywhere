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

    model_->Initialize();
}


void UnifiedDialogView::OnListSelectionChanged(
    const zaf::ListControlSelectionChangedInfo& event_info) {

    auto selected_data = listView->ListControl()->FirstSelectedItemData();
    auto dialog = zaf::As<Dialog>(selected_data);
    if (dialog) {
        OpenDialogView(std::move(dialog));
    }
}


void UnifiedDialogView::StartNewDialog(std::wstring question) {

    auto new_dialog = model_->CreateNewDialog();
    OpenDialogView(new_dialog);
    current_dialog_view_->Chat(std::move(question));

    auto dialog_index = model_->DialogDataSource()->GetIndexOfDialog(new_dialog->ID());
    if (dialog_index) {
        listView->ListControl()->SelectItemAtIndex(*dialog_index);
    }
}


void UnifiedDialogView::OpenDialogView(std::shared_ptr<Dialog> dialog) {

    if (current_dialog_view_) {
        if (current_dialog_view_->Model().Dialog()->ID() == dialog->ID()) {
            return;
        }
    }

    auto dialog_model = std::make_unique<DialogModel>(
        std::move(dialog),
        model_);

    current_dialog_view_ = zaf::Create<DialogView>(std::move(dialog_model));
    splitControl->SetSecondPane(current_dialog_view_);
    current_dialog_view_->SetFocusToInputEdit();
}

}