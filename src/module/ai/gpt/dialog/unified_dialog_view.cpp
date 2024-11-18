#include "module/ai/gpt/dialog/unified_dialog_view.h"
#include <zaf/base/as.h>
#include <zaf/input/keyboard.h>

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(UnifiedDialogView);

UnifiedDialogView::UnifiedDialogView(std::shared_ptr<UnifiedDialogModel> model) :
    model_(std::move(model)) {

}


void UnifiedDialogView::AfterParse() {

    __super::AfterParse();

    Subscriptions() += listView->NewDialogButton()->ClickEvent().Subscribe(
        std::bind(&UnifiedDialogView::OnStartNewDialogClick, this));

    listView->SetDataSource(model_->DialogDataSource());

    Subscriptions() += listView->ListControl()->SelectionChangedEvent().Subscribe(
        std::bind_front(&UnifiedDialogView::OnListSelectionChanged, this));

    Subscriptions() += listView->ListControl()->ContextMenuEvent().Subscribe(
        std::bind_front(&UnifiedDialogView::OnListContextMenu, this));

    model_->Initialize();
}


void UnifiedDialogView::OnListSelectionChanged(
    const zaf::ListControlSelectionChangedInfo& event_info) {

    auto selected_data = listView->ListControl()->FirstSelectedItemData();
    auto dialog = zaf::As<Dialog>(selected_data);
    if (dialog) {
        OpenDialogView(std::move(dialog));
    }
    else {
        ShowEmptyDialogView();
    }
}


void UnifiedDialogView::OnListContextMenu(const zaf::ListControlContextMenuInfo& event_info) {

    auto dialog = zaf::As<Dialog>(event_info.ItemData());
    if (!dialog) {
        return;
    }

    auto menu = zaf::Create<zaf::PopupMenu>();
    menu->AddMenuItem([this, dialog]() {

        auto item = zaf::Create<zaf::MenuItem>();
        item->SetText(L"Delete");

        Subscriptions() += item->MouseUpEvent().Subscribe(std::bind([this, dialog]() {
            model_->DeleteDialog(dialog->ID());
        }));
        return item;
    }());

    event_info.SetMenu(std::move(menu));
}


void UnifiedDialogView::OnStartNewDialogClick() {
    StartNewDialog({});
}


void UnifiedDialogView::StartNewDialog(std::wstring question) {

    auto new_dialog = model_->DialogService()->CreateNewDialog();
    OpenDialogView(new_dialog);
    current_dialog_view_->Chat(std::move(question));

    auto dialog_index = model_->DialogDataSource()->GetIndexOfDialog(new_dialog->ID());
    if (dialog_index) {
        listView->ListControl()->SelectItemAtIndex(*dialog_index);
        listView->ListControl()->ScrollToItemAtIndex(*dialog_index);
    }
}


void UnifiedDialogView::OpenDialogView(std::shared_ptr<Dialog> dialog) {

    if (current_dialog_view_) {
        if (current_dialog_view_->Model().Dialog()->ID() == dialog->ID()) {
            return;
        }
    }

    auto dialog_model = std::make_shared<DialogModel>(
        std::move(dialog),
        model_);

    dialog_model->Initialize();

    current_dialog_view_ = zaf::Create<DialogView>(std::move(dialog_model));
    splitControl->SetSecondPane(current_dialog_view_);
    current_dialog_view_->SetFocusToInputEdit();
}


void UnifiedDialogView::ShowEmptyDialogView() {

    current_dialog_view_.reset();

    if (!empty_dialog_view_) {
        empty_dialog_view_ = zaf::Create<Control>();
    }

    splitControl->SetSecondPane(empty_dialog_view_);
}


void UnifiedDialogView::HandleWindowMessage(const zaf::MessageReceivedInfo& event_info) {

    if (event_info.Message().ID() == WM_KEYDOWN) {
        if (HandleKeyDownMessage(zaf::KeyMessage{ event_info.Message() })) {
            event_info.MarkAsHandled(0);
        }
    }
}


bool UnifiedDialogView::HandleKeyDownMessage(const zaf::KeyMessage& message) {

    if (message.Key() == zaf::Key::N && zaf::Keyboard::IsCtrlDown()) {
        StartNewDialog({});
        return true;
    }

    return false;
}

}