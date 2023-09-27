#include "help/markdown/render/unordered_list_region.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>
#include "help/markdown/render/list_item_region.h"

namespace ra::help::markdown::render {

std::shared_ptr<UnorderedListRegion> UnorderedListRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::UnorderedList);

    std::vector<std::shared_ptr<RenderRegion>> item_regions;
    for (const auto& each_item : element.Children()) {

        auto item_region = ListItemRegion::Create(L"\u2022", *each_item, style_config);
        item_regions.push_back(std::move(item_region));
    }

    return zaf::Init(new UnorderedListRegion(std::move(item_regions)));
}


UnorderedListRegion::UnorderedListRegion(
    std::vector<std::shared_ptr<RenderRegion>> item_regions)
    :
    CompoundRegion(std::move(item_regions)) {

}

}