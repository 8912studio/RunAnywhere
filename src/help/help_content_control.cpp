#include "help/help_content_control.h"
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/render/markdown_region.h"

using namespace ra::help::markdown;

namespace ra::help {

ZAF_DEFINE_TYPE(HelpContentControl)
ZAF_DEFINE_TYPE_END


void HelpContentControl::AfterParse() {

    __super::AfterParse();

    this->SetPadding(zaf::Frame{ 12, 12, 4, 12 });

    auto root = element::MakeRoot({
        element::MakeParagraph({
            element::MakeText(L"This is a "),
            element::MakeBold(L"BOLD"),
            element::MakeText(L" text!"),
        }),
        element::MakeParagraph({
            element::MakeText(L"This is an "),
            element::MakeInlineCode(L"inline code"),
            element::MakeText(L"."),
        }),
        element::MakeCodeBlock(L"This a code block.\nThis is the second line."),
    });

    render::StyleConfig style_config;
    style_config.basic_config.font = zaf::Font::Default();
    style_config.basic_config.text_color = zaf::Color::Black();
    style_config.bold_font_weight = zaf::FontWeight::Bold;
    style_config.code_config.font_family_name = L"Consolas";
    style_config.code_config.text_color = zaf::Color::Blue();

    markdown_region_ = render::MarkdownRegion::Create(*root, style_config);
    this->AddChild(markdown_region_);
}


void HelpContentControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    if (markdown_region_) {
        markdown_region_->SetRect(zaf::Rect(zaf::Point(), ContentSize()));
    }
}


void HelpContentControl::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);

    /*
    auto content_rect = this->ContentRect();
    float start_y = content_rect.position.y;
    if (content_rect.size.height > content_layouter_.GetTotalHeight()) {
        start_y += (content_rect.size.height - content_layouter_.GetTotalHeight()) / 2;
    }

    for (const auto& each_layout_info : content_layouter_.GetLayoutInfos()) {

        zaf::Point position;
        position.x = content_rect.position.x + each_layout_info.horizontal_info.x;
        position.y = start_y + each_layout_info.y;

        canvas.SetBrushWithColor(each_layout_info.horizontal_info.color);
        canvas.DrawTextLayout(each_layout_info.horizontal_info.text_layout, position);
    }
    */
}


void HelpContentControl::OnRectChanged(const zaf::RectChangedInfo& event_info) {

    __super::OnRectChanged(event_info);

    if (this->Width() != event_info.PreviousRect().size.width) {

        content_layouter_.SetLayoutWidth(this->ContentSize().width);
        UpdateHeight();
    }
}


void HelpContentControl::SetContent(const content::Content& content) {

    content_layouter_.Layout(content);
    UpdateHeight();
    NeedRepaint();
}


void HelpContentControl::UpdateHeight() {

    float height = content_layouter_.GetTotalHeight();
    height += this->Padding().top + this->Padding().bottom;

    if (this->Height() != height) {
        need_update_height_event_.GetObserver().OnNext(height);
    }
}

}