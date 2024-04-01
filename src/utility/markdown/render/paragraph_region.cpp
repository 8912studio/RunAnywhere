#include "utility/markdown/render/paragraph_region.h"
#include <zaf/base/error/contract.h>
#include <zaf/creation.h>

namespace ra::utility::markdown::render {

std::shared_ptr<ParagraphRegion> ParagraphRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Paragraph);

    auto result = zaf::Init(new ParagraphRegion());
    result->InitializeContent(element, style_config);
    result->InitializeStyle(style_config);
    return result;
}


void ParagraphRegion::InitializeStyle(const StyleConfig& style_config) {
    TextBox()->SetLineSpacingByParagraphStyle(style_config);
}

}