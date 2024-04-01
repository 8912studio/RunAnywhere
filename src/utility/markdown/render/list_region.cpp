#include "utility/markdown/render/list_region.h"
#include <functional>
#include <zaf/base/as.h>
#include <zaf/base/error/contract.h>
#include <zaf/base/range.h>
#include <zaf/creation.h>
#include "utility/markdown/element/list_element.h"
#include "utility/markdown/render/list_item_region.h"

namespace ra::utility::markdown::render {
namespace {

using ItemIdentityCreator = std::function<std::wstring(std::size_t index, std::size_t depth)>;

std::wstring UnorderedListItemIdentityCreator(std::size_t index, std::size_t depth) {
    return depth % 2 ? L"\u25e6" : L"\u2022";
}

std::wstring OrderedListItemIdentityCreator(
    std::size_t first_number,
    std::size_t index,
    std::size_t depth) {

    return std::to_wstring(first_number + index) + L'.';
}

}

std::shared_ptr<ListRegion> ListRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config,
    std::size_t depth) {

    ZAF_EXPECT(
        element.Type() == element::ElementType::UnorderedList || 
        element.Type() == element::ElementType::OrderedList);

    auto& list_element = zaf::As<element::ListElement>(element);

    zaf::Frame item_margin;
    if (list_element.ItemStyle() == element::ListItemStyle::Blocks) {
        item_margin.top = style_config.block_gap - style_config.paragraph_config.line_gap;
    }

    ItemIdentityCreator identity_creator;
    if (element.Type() == element::ElementType::UnorderedList) {
        identity_creator = UnorderedListItemIdentityCreator;
    }
    else {
        const auto& ordered_list_element = zaf::As<element::OrderedListElement>(element);
        identity_creator = std::bind(
            OrderedListItemIdentityCreator, 
            ordered_list_element.FirstNumber(),
            std::placeholders::_1,
            std::placeholders::_2);
    }

    std::vector<std::shared_ptr<RenderRegion>> item_regions;
    for (auto index : zaf::Range(0, list_element.Children().size())) {

        const auto& each_child = list_element.Children()[index];
        auto item_region = ListItemRegion::Create(
            identity_creator(index, depth),
            *each_child,
            style_config,
            depth);

        if (index != 0) {
            item_region->SetMargin(item_margin);
        }

        item_regions.push_back(std::move(item_region));
    }

    return zaf::Init(new ListRegion(std::move(item_regions)));
}


ListRegion::ListRegion(std::vector<std::shared_ptr<RenderRegion>> item_regions) :
    CompoundRegion(std::move(item_regions)) {

}

}