#include "help/markdown/render/paragraph_region.h"
#include <zaf/base/error/check.h>

namespace ra::help::markdown::render {

void ParagraphRegion::Initialize(const element::Element& element, const TextStyle& text_style) {

    ZAF_EXPECT(element.Type() == element::ElementType::Paragraph);
}

}
