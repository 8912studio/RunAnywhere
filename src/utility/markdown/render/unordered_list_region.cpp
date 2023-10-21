#include "utility/markdown/render/unordered_list_region.h"
#include <zaf/base/as.h>
#include <zaf/base/error/check.h>
#include <zaf/base/range.h>
#include <zaf/creation.h>
#include "utility/markdown/element/list_element.h"
#include "utility/markdown/render/list_item_region.h"

namespace ra::help::markdown::render {

std::shared_ptr<UnorderedListRegion> UnorderedListRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config,
    std::size_t depth) {

    ZAF_EXPECT(element.Type() == element::ElementType::UnorderedList);
    auto& list_element = zaf::As<element::ListElement>(element);

    zaf::Frame item_margin;
    if (list_element.ItemStyle() == element::ListItemStyle::Blocks) {
        item_margin.top = style_config.block_gap - style_config.paragraph_config.line_gap;
    }

    std::vector<std::shared_ptr<RenderRegion>> item_regions;
    for (auto index : zaf::Range(0, list_element.Children().size())) {

        const auto& each_child = list_element.Children()[index];
        auto item_region = ListItemRegion::Create(
            depth % 2 ? L"\u25e6" : L"\u2022",
            *each_child,
            style_config,
            depth);

        if (index != 0) {
            item_region->SetMargin(item_margin);
        }

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