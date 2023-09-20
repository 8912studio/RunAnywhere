#include "help/markdown/render/paragraph_region.h"
#include <zaf/base/error/check.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/text/text_format_properties.h>
#include <zaf/graphic/text/text_layout_properties.h>

namespace ra::help::markdown::render {
namespace {

zaf::TextLayout CreateTextLayout(const StyledText& styled_text) {

    zaf::TextFormatProperties text_format_properties;
    auto text_format = zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);

    zaf::TextLayoutProperties text_layout_properties;
    text_layout_properties.text_format = text_format;
    text_layout_properties.text = styled_text.Text();

    auto result = zaf::GraphicFactory::Instance().CreateTextLayout(text_layout_properties);
    for (const auto& each_style : styled_text.Styles()) {
        result.SetFontFamilyName(each_style.style.font.family_name, each_style.range);
        result.SetFontSize(each_style.style.font.size, each_style.range);
        result.SetFontWeight(each_style.style.font.weight, each_style.range);
    }

    return result;
}

}

ParagraphRegion::ParagraphRegion(const StyledText& styled_text) :
    text_layout_(CreateTextLayout(styled_text)) {

}


void ParagraphRegion::Resize(const zaf::Size& size) {

    text_layout_.SetMaxWidth(size.width);
    text_layout_.SetMaxHeight(size.height);
}


void ParagraphRegion::Paint(zaf::Canvas& canvas) {

    canvas.SetBrushWithColor(zaf::Color::Black());
    canvas.DrawTextLayout(text_layout_, {});
}

}
