#include "utility/markdown/render/list_item_region.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>

namespace ra::utility::markdown::render {

std::shared_ptr<ListItemRegion> ListItemRegion::Create(
    const std::wstring& item_marker,
    const element::Element& element, 
    const StyleConfig& style_config,
    std::size_t depth) {

    ZAF_EXPECT(element.Type() == element::ElementType::ListItem);

    auto body_region = BodyRegion::Create(element.Children(), style_config, depth + 1);
    auto result = zaf::Init(new ListItemRegion(std::move(body_region)));
    result->InitializeStyle(item_marker, style_config);
    return result;
}


void ListItemRegion::InitializeStyle(
    const std::wstring& marker, 
    const StyleConfig& style_config) {

    marker_text_box_->SetText(marker);
    marker_text_box_->SetFont(style_config.basic_config.font);
    marker_text_box_->SetTextColor(style_config.basic_config.text_color);
    marker_text_box_->SetLineSpacingByParagraphStyle(style_config);

    indent_ = style_config.list_config.indent;
    marker_gap_ = style_config.list_config.item_marker_gap;
}


ListItemRegion::ListItemRegion(std::shared_ptr<BodyRegion> body_region) :
    body_region_(std::move(body_region)) {
    
}


void ListItemRegion::Initialize() {

    __super::Initialize();

    marker_text_box_ = zaf::Create<StyledTextBox>();
    AddChild(marker_text_box_);

    AddChild(body_region_);
}


void ListItemRegion::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    zaf::Rect marker_rect{
        zaf::Point(indent_, 0),
        marker_text_box_->CalculatePreferredSize(),
    };
    marker_text_box_->SetRect(marker_rect);

    zaf::Rect body_rect(zaf::Point(), ContentSize());
    body_rect.Deflate(zaf::Frame(marker_rect.Right() + marker_gap_, 0, 0, 0));
    body_region_->SetRect(body_rect);
}


zaf::Size ListItemRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    auto marker_size = marker_text_box_->CalculatePreferredSize();
    float non_body_width = indent_ + marker_size.width + marker_gap_;

    auto body_bound_size = bound_size;
    body_bound_size.width -= non_body_width;

    auto body_size = body_region_->CalculatePreferredSize(body_bound_size);

    return zaf::Size{
        non_body_width + body_size.width,
        std::max(marker_size.height, body_size.height)
    };
}


bool ListItemRegion::IsPositionInsideTextBoundary(const zaf::Point& mouse_position) {
    
    auto position_in_marker = this->TranslateToChild(mouse_position, *marker_text_box_);
    if (marker_text_box_->RectInSelf().Contain(position_in_marker)) {
        return marker_text_box_->IsPositionInsideText(position_in_marker);
    }

    return body_region_->IsPositionInsideTextBoundary(
        this->TranslateToChild(mouse_position, *body_region_));
}


void ListItemRegion::BeginSelection(const zaf::Point& position) {
    body_region_->BeginSelection(this->TranslateToChild(position, *body_region_));
}


void ListItemRegion::ChangeSelection(const composite::PositionRange& position_range) {

    ChangeSelectionOfMarker(composite::PositionRange{
        this->TranslateToChild(position_range.Begin(), *marker_text_box_),
        this->TranslateToChild(position_range.End(), *marker_text_box_)
    });

    body_region_->ChangeSelection(composite::PositionRange{
        this->TranslateToChild(position_range.Begin(), *body_region_),
        this->TranslateToChild(position_range.End(), *body_region_)
    });
}


void ListItemRegion::EndSelection() {
    body_region_->EndSelection();
}


void ListItemRegion::SelectWord(const zaf::Point& position) {
    body_region_->SelectWord(this->TranslateToChild(position, *body_region_));
}


void ListItemRegion::ChangeSelectionOfMarker(const composite::PositionRange& position_range) {

    auto has_selection = [this, &position_range]() {

        auto sorted = position_range.Sort();
        const auto& begin_position = sorted.first;
        const auto& end_position = sorted.second;
    
        //There is no intersection between the range and the marker, no selection.
        if (begin_position.y >= marker_text_box_->Height() ||
            end_position.y < 0) {
            return false;
        }

        //The marker is selected if the y of begin position is above the marker.
        if (begin_position.y < 0) {
            return true;
        }

        //No selection is the begin postion and the end position are identical.
        if (begin_position == end_position) {
            return false;
        }

        //Finally, the marker is selected if either the begin position or the end position is in 
        //the rectangle of marker.
        auto begin_index = marker_text_box_->FindIndexAtPosition(begin_position);
        if (begin_index < marker_text_box_->TextLength()) {
            return true;
        }

        auto end_index = marker_text_box_->FindIndexAtPosition(end_position);
        if (end_index < marker_text_box_->TextLength()) {
            return true;
        }

        return false;
    };

    marker_text_box_->SetSelectionRange(
        has_selection() ? zaf::Range{ 0, marker_text_box_->TextLength() } : zaf::Range{});
}


void ListItemRegion::BuildSelectedText(composite::SelectedTextBuilder& builder) {

    auto marker_text = marker_text_box_->SelectedText();
    if (!marker_text.empty()) {
        builder.Append(marker_text);
        builder.Append(L" ");
    }

    body_region_->BuildSelectedText(builder);
}

void ListItemRegion::ChangeFocus(bool is_focused) {

    marker_text_box_->ChangeFocus(is_focused);
    body_region_->ChangeFocus(is_focused);
}

}