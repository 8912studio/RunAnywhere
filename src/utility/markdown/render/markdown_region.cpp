#include "utility/markdown/render/markdown_region.h"
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>

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


zaf::Size MarkdownRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {
    return body_region_->CalculatePreferredSize(bound_size);
}



void MarkdownRegion::ChangeSelection(
    const zaf::Point& current_position, 
    const zaf::Point& begin_position) {

}


void MarkdownRegion::OnMouseDown(const zaf::PreMouseDownInfo& event_info) {

    __super::OnMouseDown(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    auto position_in_body = this->TranslatePositionToChild(
        event_info.PositionAtSender(),
        *body_region_);

    begin_selection_position_ = position_in_body;
    body_region_->ChangeSelection(*begin_selection_position_, *begin_selection_position_);

    CaptureMouse();
    event_info.MarkAsHandled();
}


void MarkdownRegion::OnMouseUp(const zaf::PreMouseUpInfo& event_info) {

    __super::OnMouseUp(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (begin_selection_position_) {
        begin_selection_position_.reset();
        ReleaseMouse();
        event_info.MarkAsHandled();
    }
}


void MarkdownRegion::OnMouseMove(const zaf::PreMouseMoveInfo& event_info) {

    __super::OnMouseMove(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    if (!begin_selection_position_) {
        return;
    }

    auto current_position = this->TranslatePositionToChild(
        event_info.PositionAtSender(), 
        *body_region_);

    zaf::Point begin_position;
    zaf::Point end_position;

    if (std::tie(current_position.y, current_position.x) < 
        std::tie(begin_selection_position_->y, begin_selection_position_->x)) {

        begin_position = current_position;
        end_position = *begin_selection_position_;
    }
    else {
        begin_position = *begin_selection_position_;
        end_position = current_position;
    }

    body_region_->ChangeSelection(begin_position, end_position);
    event_info.MarkAsHandled();
}

}