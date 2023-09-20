#include "help/markdown/render/styled_text_builder.h"
#include <zaf/base/error/check.h>

namespace ra::help::markdown::render {
namespace {

TextStyle CreateNewStyleByElement(const element::Element& element, const TextStyle& basic_style) {

    auto new_style = basic_style;

    switch (element.Type()) {
    case element::ElementType::Paragraph:
        //No need to change style of paragraph.
        break;
    case element::ElementType::Bold:
        new_style.font.weight = zaf::FontWeight::Bold;
        break;
    default:
        ZAF_NOT_REACHED();
    }

    return new_style;
}

}

StyledText StyledTextBuilder::Build(
    const element::Element& element, 
    const TextStyle& basic_style) {

    StyledText result;
    BuildElement(element, basic_style, result);

    ZAF_EXPECT(result.CheckConstraint());
    return result;
}


void StyledTextBuilder::BuildElement(
    const element::Element& element, 
    const TextStyle& current_style,
    StyledText& result) {

    if (element.IsTextElement()) {
        result.Append(element.Text());
        return;
    }

    result.AddStyleToPendingText(current_style);

    auto new_style = CreateNewStyleByElement(element, current_style);

    for (const auto& each_child : element.Children()) {
        BuildElement(*each_child, new_style, result);
    }

    result.AddStyleToPendingText(new_style);
}

}