#include "help/content_layouter.h"
#include <zaf/application.h>
#include <zaf/base/as.h>
#include <zaf/base/container/utility/append.h>
#include <zaf/base/container/utility/range.h>
#include <zaf/graphic/font/font_weight.h>
#include <zaf/graphic/resource_factory.h>
#include <zaf/graphic/text/text_format_properties.h>
#include <zaf/graphic/text/text_layout_properties.h>

namespace ra::help {
namespace {

class LayoutProperties {
public:
    bool is_bold{};
    bool is_italic{};
    bool is_right_alignment{};
};


zaf::TextFormat CreateTextFormat(const LayoutProperties& layout_properties) {

    zaf::TextFormatProperties text_format_properties;
    text_format_properties.font_size = 16;
    text_format_properties.font_weight = 
        layout_properties.is_bold ? zaf::FontWeight::Bold : zaf::FontWeight::Regular;

    auto text_format = zaf::GetGraphicFactory().CreateTextFormat(text_format_properties);
    text_format.SetParagraphAlignment(zaf::ParagraphAlignment::Top);
    text_format.SetTextAlignment(
        layout_properties.is_right_alignment ? 
        zaf::TextAlignment::Right :
        zaf::TextAlignment::Left);
    text_format.SetWordWrapping(zaf::WordWrapping::Wrap);

    return text_format;
}


zaf::TextLayout CreateTextLayout(
    const std::wstring& text, 
    float layout_width, 
    const LayoutProperties& layout_properties) {

    zaf::TextLayoutProperties text_layout_properties;
    text_layout_properties.text = text;
    text_layout_properties.text_format = CreateTextFormat(layout_properties);

    auto text_layout = zaf::GetGraphicFactory().CreateTextLayout(text_layout_properties);

    text_layout.SetFontStyle(
        layout_properties.is_italic ? zaf::FontStyle::Italic : zaf::FontStyle::Normal,
        zaf::TextRange{ 0, text.length() });

    text_layout.SetMaxWidth(layout_width);
    text_layout.SetMaxHeight(10000);
    return text_layout;
}

}

void ContentLayouter::SetLayoutWidth(float width) {

    layout_width_ = width;
    Relayout();
}


void ContentLayouter::Layout(const content::Content& content) {

    content_ = content;
    Relayout();
}


void ContentLayouter::Relayout() {

    current_y_ = 0;
    layout_infos_.clear();

    for (std::size_t index : zaf::Range(0u, content_.Lines().size())) {

        //Add line margin.
        if (index != 0) {
            current_y_ += 14;
        }

        auto horizontal_infos = CreateLineLayoutInfos(*content_.Lines()[index]);

        float height{};
        for (const auto& each_info : horizontal_infos) {

            height = std::max(height, each_info.text_layout.GetMetrics().Height());

            CellVerticalLayoutInfo vertical_layout_info;
            vertical_layout_info.y = current_y_;
            vertical_layout_info.horizontal_info = each_info;
            layout_infos_.push_back(vertical_layout_info);
        }

        current_y_ += height;
    }
}


std::vector<CellHorizontalLayoutInfo> ContentLayouter::CreateLineLayoutInfos(
    const content::Line& line) {

    if (auto title_line = zaf::As<content::TitleLine>(&line)) {
        return CreateTitleLineLayoutInfos(*title_line);
    }

    if (auto body_line = zaf::As<content::BodyLine>(&line)) {
        return CreateBodyLineLayoutInfos(*body_line);
    }

    if (auto two_parts_line = zaf::As<content::TwoPartsLine>(&line)) {
        return CreateTwoPartsLineLayoutInfos(*two_parts_line);
    }

    return {};
}


std::vector<CellHorizontalLayoutInfo> ContentLayouter::CreateTitleLineLayoutInfos(
    const content::TitleLine& title_line) {

    CellHorizontalLayoutInfo layout_info;
    LayoutProperties layout_properties;
    layout_properties.is_bold = true;
    layout_info.text_layout = CreateTextLayout(title_line.Text(), layout_width_, layout_properties);
    return { layout_info };
}


std::vector<CellHorizontalLayoutInfo> ContentLayouter::CreateBodyLineLayoutInfos(
    const content::BodyLine& body_line) {

    CellHorizontalLayoutInfo layout_info;
    layout_info.text_layout = CreateTextLayout(body_line.Text(), layout_width_, LayoutProperties{});
    return { layout_info };
}


std::vector<CellHorizontalLayoutInfo> ContentLayouter::CreateTwoPartsLineLayoutInfos(
    const content::TwoPartsLine& two_parts_line) {

    constexpr float PrimaryPartWidth = 100;

    CellHorizontalLayoutInfo primary_part_layout_info;
    primary_part_layout_info.text_layout = CreateTextLayout(
        two_parts_line.PrimaryText(), 
        PrimaryPartWidth,
        LayoutProperties{});

    CellHorizontalLayoutInfo secondly_part_layout_info;
    secondly_part_layout_info.x = PrimaryPartWidth;
    secondly_part_layout_info.color = zaf::Color::FromRGB(0x666666);

    LayoutProperties secondly_layout_properties;
    secondly_layout_properties.is_italic = true;
    secondly_part_layout_info.text_layout = CreateTextLayout(
        two_parts_line.SecondlyText(), 
        std::max(0.f, layout_width_ - PrimaryPartWidth), 
        secondly_layout_properties);

    return { primary_part_layout_info, secondly_part_layout_info };
}

}