#include "help/markdown/render/paragraph_region.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>
#include <zaf/graphic/canvas.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/text/text_format_properties.h>
#include <zaf/graphic/text/text_layout_properties.h>
#include "help/markdown/render/styled_text_builder.h"

namespace ra::help::markdown::render {
namespace {

StyledTextLayout CreateStyledTextLayout(const StyledText& styled_text) {

    zaf::TextFormatProperties text_format_properties;
    auto text_format = zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);

    zaf::TextLayoutProperties text_layout_properties;
    text_layout_properties.text_format = text_format;
    text_layout_properties.text = styled_text.Text();

    auto text_layout = zaf::GraphicFactory::Instance().CreateTextLayout(text_layout_properties);

    for (const auto& each_style : styled_text.Styles()) {
        text_layout.SetFontFamilyName(each_style.style.font.family_name, each_style.range);
        text_layout.SetFontSize(each_style.style.font.size, each_style.range);
        text_layout.SetFontWeight(each_style.style.font.weight, each_style.range);
    }

    StyledTextLayout result;
    result.text_layout = std::move(text_layout);
    return result;
}

}

std::shared_ptr<ParagraphRegion> ParagraphRegion::Create(
    const element::Element& element,
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Paragraph);

    StyledTextBuilder styled_text_builder;
    auto styled_text = styled_text_builder.Build(element, style_config);

    auto styled_text_layout = CreateStyledTextLayout(styled_text);

    return zaf::Create<ParagraphRegion>(std::move(styled_text_layout));
}


ParagraphRegion::ParagraphRegion(StyledTextLayout styled_text_layout) :
    styled_text_layout_(styled_text_layout) {

}


void ParagraphRegion::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    auto content_size = ContentSize();
    styled_text_layout_.text_layout.SetMaxWidth(content_size.width);
    styled_text_layout_.text_layout.SetMaxHeight(content_size.height);
}


void ParagraphRegion::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);

    canvas.SetBrushWithColor(zaf::Color::Black());
    canvas.DrawTextLayout(styled_text_layout_.text_layout, ContentRect().position);
}


zaf::Size ParagraphRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    auto old_width = styled_text_layout_.text_layout.GetMaxWidth();
    auto old_height = styled_text_layout_.text_layout.GetMaxHeight();

    styled_text_layout_.text_layout.SetMaxWidth(bound_size.width);
    styled_text_layout_.text_layout.SetMaxHeight(bound_size.height);

    auto metrics = styled_text_layout_.text_layout.GetMetrics();

    styled_text_layout_.text_layout.SetMaxWidth(old_width);
    styled_text_layout_.text_layout.SetMaxHeight(old_height);

    return zaf::Size{ metrics.Width(), metrics.Height()};
}

}
