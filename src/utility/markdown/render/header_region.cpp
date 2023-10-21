#include "utility/markdown/render/header_region.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>

namespace ra::help::markdown::render {

std::shared_ptr<HeaderRegion> HeaderRegion::Create(
    const element::Element& element,
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Header);

    auto result = zaf::Init(new HeaderRegion());
    result->InitializeContent(element, style_config);
    return result;
}


zaf::Size HeaderRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    //The height calculated for single line bold text might be not enough to contain the actual 
    //text. We have to add some extra height to solve it.
    auto result = __super::CalculatePreferredContentSize(bound_size);
    result.height = std::ceil(result.height);
    return result;
}

}