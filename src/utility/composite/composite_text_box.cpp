#include "utility/composite/composite_text_box.h"
#include <zaf/object/type_definition.h>
#include "utility/clipboard.h"

namespace ra::utility::composite {

ZAF_DEFINE_TYPE(CompositeTextBox)
ZAF_DEFINE_TYPE_END;


void CompositeTextBox::Initialize() {

    __super::Initialize();

    this->SetCanFocused(true);
    this->SetCanDoubleClick(true);
}


void CompositeTextBox::OnMouseCursorChanging(
    const zaf::MouseCursorChangingInfo& event_info) {

    __super::OnMouseCursorChanging(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (this->IsPositionInsideTextBoundary(this->GetMousePosition())) {
        SetCursor(LoadCursor(nullptr, IDC_IBEAM));
        event_info.MarkAsHandled();
    }
}


void CompositeTextBox::OnMouseDown(const zaf::MouseDownInfo& event_info) {

    __super::OnMouseDown(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    this->SetIsFocused(true);
    this->ChangeFocus(this->IsFocused());

    auto mouse_button = event_info.Message().MouseButton();
    if (mouse_button == zaf::MouseButton::Left) {
        HandleLeftButtonDown(event_info);
    }
    else if (mouse_button == zaf::MouseButton::Right) {
        HandleRightButtonDown(event_info);
    }
    else {
        return;
    }

    event_info.MarkAsHandled();
}


void CompositeTextBox::HandleLeftButtonDown(const zaf::MouseDownInfo& event_info) {

    begin_selection_position_ = event_info.PositionAtSender();

    this->BeginSelection(*begin_selection_position_);

    InnerChangeSelection(PositionRange{
        *begin_selection_position_,
        *begin_selection_position_
    });

    CaptureMouse();
}


void CompositeTextBox::HandleRightButtonDown(const zaf::MouseDownInfo& event_info) {

    //Pop up the menu only when the clicked position is inside text.
    if (!this->IsPositionInsideTextBoundary(event_info.PositionAtSender())) {
        return;
    }

    auto copy_menu_item = zaf::Create<zaf::MenuItem>();
    copy_menu_item->SetText(L"Copy");
    Subscriptions() += copy_menu_item->MouseUpEvent().Subscribe(
        [this](const zaf::MouseUpInfo& event_info) {

        CopySelectionToClipboard();
        event_info.MarkAsHandled();
    });

    auto menu = zaf::Create<zaf::PopupMenu>();
    menu->AddMenuItem(copy_menu_item);

    Subscriptions() += menu->DestroyedEvent().Subscribe(std::bind([this]() {
        this->ChangeFocus(this->IsFocused());
    }));

    popup_menu_ = menu;
    menu->PopupOnControl(shared_from_this(), event_info.PositionAtSender());
}


void CompositeTextBox::InnerChangeSelection(const PositionRange& position_range) {

    this->ChangeSelection(position_range);
    selection_changed_event_.Raise(CompositeTextBoxSelectionChangedInfo{ position_range });
}


void CompositeTextBox::CopySelectionToClipboard() {

    SelectedTextBuilder text_builder;
    this->BuildSelectedText(text_builder);

    if (!text_builder.Text().empty()) {
        SetStringToClipboard(text_builder.Text());
    }
}


void CompositeTextBox::OnMouseUp(const zaf::MouseUpInfo& event_info) {

    __super::OnMouseUp(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (begin_selection_position_) {
        begin_selection_position_.reset();

        this->EndSelection();
        ReleaseMouse();
        event_info.MarkAsHandled();
    }
}


void CompositeTextBox::OnPreMouseMove(const zaf::PreMouseMoveInfo& event_info) {

    __super::OnPreMouseMove(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!begin_selection_position_) {
        return;
    }

    auto current_position = event_info.PositionAtSender();

    InnerChangeSelection(PositionRange{
        *begin_selection_position_,
        current_position
    });

    event_info.MarkAsHandled();
}


void CompositeTextBox::OnDoubleClick(const zaf::DoubleClickInfo& event_info) {

    __super::OnDoubleClick(event_info);

    this->SelectWord(event_info.Position());
}


void CompositeTextBox::OnKeyDown(const zaf::KeyDownInfo& event_info) {

    __super::OnKeyDown(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if ((GetKeyState(VK_CONTROL) >> 15) && event_info.Message().VirtualKey() == L'C') {
        CopySelectionToClipboard();
        event_info.MarkAsHandled();
    }
}


void CompositeTextBox::OnFocusGained(const zaf::FocusGainedInfo& event_info) {

    __super::OnFocusGained(event_info);

    this->ChangeFocus(true);
    event_info.MarkAsHandled();
}


void CompositeTextBox::OnFocusLost(const zaf::FocusLostInfo& event_info) {

    __super::OnFocusLost(event_info);

    if (!popup_menu_.expired()) {
        return;
    }

    this->ChangeFocus(false);
    event_info.MarkAsHandled();
}

}