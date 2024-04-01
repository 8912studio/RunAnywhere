#include "utility/markdown/render/styled_text_builder.h"
#include <zaf/base/as.h>
#include <zaf/base/error/check.h>
#include "utility/markdown/element/header_element.h"

using namespace zaf::textual;

namespace ra::utility::markdown::render {

StyledText StyledTextBuilder::Build(
    const element::Element& element, 
    const StyleConfig& style_config) {

    zaf::textual::TextStyle basic_style;
    basic_style.SetFont(style_config.basic_config.font);
    basic_style.SetTextColor(style_config.basic_config.text_color);

    StyledText result;
    BuildElement(element, basic_style, style_config, result);
    return result;
}


void StyledTextBuilder::BuildElement(
    const element::Element& element, 
    const TextStyle& current_style,
    const StyleConfig& style_config,
    StyledText& result) {

    if (element.IsTextElement()) {
        result.AppendText(element.Text(), current_style);
        return;
    }

    auto new_style = CreateNewStyleByElement(element, current_style, style_config);

    for (const auto& each_child : element.Children()) {
        BuildElement(*each_child, new_style, style_config, result);
    }
}


TextStyle StyledTextBuilder::CreateNewStyleByElement(
    const element::Element& element, 
    const TextStyle& basic_style,
    const StyleConfig& style_config) {

    auto new_style = basic_style;

    switch (element.Type()) {

    case element::ElementType::Paragraph:
        //No need to change style of paragraph.
        break;

    case element::ElementType::Header:
        SetNewStyleByHeader(element, style_config, new_style);
        break;

    case element::ElementType::Bold:
        new_style.Font()->weight = style_config.bold_font_weight;
        break;

    case element::ElementType::Italic:
        new_style.Font()->style = zaf::FontStyle::Italic;
        break;

    case element::ElementType::InlineCode:
        new_style.Font()->family_name = style_config.inline_code_config.font_family_name;
        new_style.SetTextColor(style_config.inline_code_config.text_color);
        new_style.SetTextBackColor(style_config.inline_code_config.background_color);
        break;

    default:
        ZAF_NOT_REACHED();
    }

    return new_style;
}


void StyledTextBuilder::SetNewStyleByHeader(
    const element::Element& element, 
    const StyleConfig& style_config,
    TextStyle& new_style) {

    const auto& header_element = dynamic_cast<const element::HeaderElement&>(element);
    auto new_font = new_style.Font();
    new_font->size = style_config.GetHeaderConfig(header_element.Depth()).font_size;
    new_font->weight = style_config.bold_font_weight;
}

}