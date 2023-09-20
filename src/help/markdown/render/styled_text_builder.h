#pragma once

#include "help/markdown/element/element.h"
#include "help/markdown/render/styled_text.h"

namespace ra::help::markdown::render {

class StyledTextBuilder {
public:
    StyledText Build(const element::Element& element, const TextStyle& basic_style);

private:
    void BuildElement(
        const element::Element& element,
        const TextStyle& current_style,
        StyledText& result);
};

}