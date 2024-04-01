#pragma once

#include <zaf/control/textual/styled_text.h>
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/style_config.h"

namespace ra::utility::markdown::render {

class StyledTextBuilder {
public:
    zaf::textual::StyledText Build(
        const element::Element& element,
        const StyleConfig& style_config);

private:
    static void BuildElement(
        const element::Element& element,
        const zaf::textual::TextStyle& current_style,
        const StyleConfig& style_config,
        zaf::textual::StyledText& result);

    static zaf::textual::TextStyle CreateNewStyleByElement(
        const element::Element& element,
        const zaf::textual::TextStyle& basic_style,
        const StyleConfig& style_config);

    static void SetNewStyleByHeader(
        const element::Element& element, 
        const StyleConfig& style_config,
        zaf::textual::TextStyle& new_style);
};

}