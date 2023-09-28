#include "help/markdown/render/list_item_region.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>

namespace ra::help::markdown::render {

std::shared_ptr<ListItemRegion> ListItemRegion::Create(
    const std::wstring& item_identity,
    const element::Element& element, 
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::ListItem);

    auto body_region = BodyRegion::Create(element.Children(), style_config);
    auto result = zaf::Init(new ListItemRegion(std::move(body_region)));
    result->InitializeStyle(item_identity, style_config);
    return result;
}


void ListItemRegion::InitializeStyle(
    const std::wstring& identity, 
    const StyleConfig& style_config) {

    identity_text_box_->SetText(identity);
    identity_text_box_->SetFont(style_config.basic_config.font);
    identity_text_box_->SetTextColor(style_config.basic_config.text_color);
    identity_text_box_->SetPadding(zaf::Frame{ 0, style_config.paragraph_config.line_gap, 0, 0 });

    identity_gap_ = style_config.list_item_identity_gap;
}


ListItemRegion::ListItemRegion(std::shared_ptr<BodyRegion> body_region) :
    body_region_(std::move(body_region)) {
    
}


void ListItemRegion::Initialize() {

    __super::Initialize();

    identity_text_box_ = zaf::Create<StyledTextBox>();
    AddChild(identity_text_box_);

    AddChild(body_region_);
}


void ListItemRegion::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    auto identity_size = identity_text_box_->CalculatePreferredSize();
    identity_text_box_->SetRect(zaf::Rect(zaf::Point(), identity_size));

    auto content_size = ContentSize();

    body_region_->SetRect(zaf::Rect{
        identity_size.width + identity_gap_,
        0,
        content_size.width - identity_size.width,
        content_size.height
    });
}


zaf::Size ListItemRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    auto identity_size = identity_text_box_->CalculatePreferredSize();

    auto body_bound_size = bound_size;
    body_bound_size.width -= identity_size.width - identity_gap_;

    auto body_size = body_region_->CalculatePreferredSize(body_bound_size);

    return zaf::Size{
        identity_size.width + body_size.width,
        std::max(identity_size.height, body_size.height)
    };
}

}