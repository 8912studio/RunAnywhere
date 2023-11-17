#include "utility/markdown/render/markdown_region.h"
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::utility::markdown::render {

std::shared_ptr<MarkdownRegion> MarkdownRegion::Create(
    const element::Element& element,
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Root);

    auto body_region = BodyRegion::Create(element.Children(), style_config, 0);
    return zaf::Init(new MarkdownRegion(body_region));
}


MarkdownRegion::MarkdownRegion(std::shared_ptr<BodyRegion> body_region) :
    body_region_(std::move(body_region)) {

}


void MarkdownRegion::Initialize() {

    __super::Initialize();

    this->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    AddChild(body_region_);
}


void MarkdownRegion::SetCanSelect(bool can_select) {

    can_select_ = can_select;
    this->SetCanFocused(can_select_);
}


zaf::Size MarkdownRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {
    return body_region_->CalculatePreferredSize(bound_size);
}


void MarkdownRegion::CopySelectionToClipboard() {

    SelectedTextBuilder text_builder;
    body_region_->BuildSelectedText(text_builder);

    if (!text_builder.Text().empty()) {
        SetStringToClipboard(text_builder.Text());
    }
}


void MarkdownRegion::OnMouseDown(const zaf::MouseDownInfo& event_info) {

    __super::OnMouseDown(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!can_select_) {
        return;
    }

    this->SetIsFocused(true);

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


void MarkdownRegion::HandleLeftButtonDown(const zaf::MouseDownInfo& event_info) {

    begin_selection_position_ = this->TranslatePositionToChild(
        event_info.PositionAtSender(),
        *body_region_);

    body_region_->BeginSelection(*begin_selection_position_);
    body_region_->ChangeSelection(PositionRange{
        *begin_selection_position_,
        *begin_selection_position_
    });

    CaptureMouse();
}


void MarkdownRegion::HandleRightButtonDown(const zaf::MouseDownInfo& event_info) {

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
        body_region_->ChangeFocus(this->IsFocused());
    }));

    popup_menu_ = menu;
    menu->PopupOnControl(shared_from_this(), event_info.PositionAtSender());
}


void MarkdownRegion::OnMouseUp(const zaf::MouseUpInfo& event_info) {

    __super::OnMouseUp(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!can_select_) {
        return;
    }

    if (begin_selection_position_) {
        begin_selection_position_.reset();

        body_region_->EndSelection();
        ReleaseMouse();
        event_info.MarkAsHandled();
    }
}


void MarkdownRegion::OnPreMouseMove(const zaf::PreMouseMoveInfo& event_info) {

    __super::OnPreMouseMove(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!can_select_) {
        return;
    }

    if (!begin_selection_position_) {
        return;
    }

    auto current_position = this->TranslatePositionToChild(
        event_info.PositionAtSender(),
        *body_region_);

    body_region_->ChangeSelection(PositionRange{
        *begin_selection_position_,
        current_position
    });

    event_info.MarkAsHandled();
}


void MarkdownRegion::OnKeyDown(const zaf::KeyDownInfo& event_info) {

    __super::OnKeyDown(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!can_select_) {
        return;
    }

    if ((GetKeyState(VK_CONTROL) >> 15) && event_info.Message().VirtualKey() == L'C') {
        CopySelectionToClipboard();
        event_info.MarkAsHandled();
    }
}


void MarkdownRegion::OnFocusGained(const zaf::FocusGainedInfo& event_info) {

    __super::OnFocusGained(event_info);

    if (!can_select_) {
        return;
    }

    body_region_->ChangeFocus(true);
    event_info.MarkAsHandled();
}


void MarkdownRegion::OnFocusLost(const zaf::FocusLostInfo& event_info) {

    __super::OnFocusLost(event_info);

    if (!can_select_) {
        return;
    }

    if (!popup_menu_.expired()) {
        return;
    }

    body_region_->ChangeFocus(false);
    event_info.MarkAsHandled();
}

}