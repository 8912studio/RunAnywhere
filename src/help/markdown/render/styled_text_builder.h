#pragma once

#include "help/markdown/element/element.h"
#include "help/markdown/render/style_config.h"
#include "help/markdown/render/styled_text.h"

namespace ra::help::markdown::render {

class StyledTextBuilder {
public:
    StyledText Build(const element::Element& element, const StyleConfig& style_config);

private:
    static void BuildElement(
        const element::Element& element,
        const TextStyle& current_style,
        const StyleConfig& style_config,
        StyledText& result);

    static TextStyle CreateNewStyleByElement(
        const element::Element& element,
        const TextStyle& basic_style,
        const StyleConfig& style_config);

    static void SetNewStyleByHeader(
        const element::Element& element, 
        const StyleConfig& style_config,
        TextStyle& new_style);
};

}