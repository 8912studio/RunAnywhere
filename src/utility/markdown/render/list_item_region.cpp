#include "utility/markdown/render/list_item_region.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>

namespace ra::help::markdown::render {

std::shared_ptr<ListItemRegion> ListItemRegion::Create(
    const std::wstring& item_identity,
    const element::Element& element, 
    const StyleConfig& style_config,
    std::size_t depth) {

    ZAF_EXPECT(element.Type() == element::ElementType::ListItem);

    auto body_region = BodyRegion::Create(element.Children(), style_config, depth + 1);
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

    indent_ = style_config.list_config.indent;
    identity_gap_ = style_config.list_config.item_identity_gap;
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

    zaf::Rect identity_rect{
        zaf::Point(indent_, 0),
        identity_text_box_->CalculatePreferredSize(),
    };
    identity_text_box_->SetRect(identity_rect);

    zaf::Rect body_rect(zaf::Point(), ContentSize());
    body_rect.Deflate(zaf::Frame(identity_rect.Right() + identity_gap_, 0, 0, 0));
    body_region_->SetRect(body_rect);
}


zaf::Size ListItemRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    auto identity_size = identity_text_box_->CalculatePreferredSize();
    float non_body_width = indent_ + identity_size.width + identity_gap_;

    auto body_bound_size = bound_size;
    body_bound_size.width -= non_body_width;

    auto body_size = body_region_->CalculatePreferredSize(body_bound_size);

    return zaf::Size{
        non_body_width + body_size.width,
        std::max(identity_size.height, body_size.height)
    };
}

}