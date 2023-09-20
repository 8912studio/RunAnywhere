#include "help/markdown/render/paragraph_region.h"
#include <zaf/base/error/check.h>
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

std::unique_ptr<ParagraphRegion> ParagraphRegion::Create(const element::Element& element) {

    ZAF_EXPECT(element.Type() == element::ElementType::Paragraph);

    TextStyle basic_style;
    basic_style.font = zaf::Font::Default();
    basic_style.font.size = 18;

    StyledTextBuilder styled_text_builder;
    auto styled_text = styled_text_builder.Build(element, basic_style);

    auto styled_text_layout = CreateStyledTextLayout(styled_text);

    return std::make_unique<ParagraphRegion>(std::move(styled_text_layout));
}


ParagraphRegion::ParagraphRegion(StyledTextLayout styled_text_layout) :
    styled_text_layout_(styled_text_layout) {

}


void ParagraphRegion::Resize(const zaf::Size& size) {

    styled_text_layout_.text_layout.SetMaxWidth(size.width);
    styled_text_layout_.text_layout.SetMaxHeight(size.height);
}


void ParagraphRegion::Paint(zaf::Canvas& canvas) {

    canvas.SetBrushWithColor(zaf::Color::Black());
    canvas.DrawTextLayout(styled_text_layout_.text_layout, {});
}

}
