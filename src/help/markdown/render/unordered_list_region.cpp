#include "help/markdown/render/unordered_list_region.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>

namespace ra::help::markdown::render {

std::shared_ptr<UnorderedListRegion> UnorderedListRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::UnorderedList);

    auto result = zaf::Init(new UnorderedListRegion());
    return result;
}

}