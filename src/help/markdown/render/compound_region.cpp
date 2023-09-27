#include "help/markdown/render/compound_region.h"

namespace ra::help::markdown::render {

CompoundRegion::CompoundRegion(std::vector<std::shared_ptr<RenderRegion>> child_regions) :
    child_regions_(std::move(child_regions)) {

}


void CompoundRegion::Initialize() {

    __super::Initialize();

    std::vector<std::shared_ptr<zaf::Control>> children;
    for (const auto& each_region : child_regions_) {
        children.push_back(each_region);
    }
    AddChildren(children);
}


void CompoundRegion::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    zaf::Size bound_size{};
    bound_size.width = this->ContentSize().width;
    bound_size.height = std::numeric_limits<float>::max();

    float region_y{};
    for (const auto& each_region : child_regions_) {

        region_y += each_region->Margin().top;

        zaf::Rect region_rect;
        region_rect.position.x = 0;
        region_rect.position.y = region_y;
        region_rect.size.width = bound_size.width;
        region_rect.size.height = each_region->CalculatePreferredSize(bound_size).height;
        each_region->SetRect(region_rect);

        region_y += region_rect.size.height + each_region->Margin().bottom;
    }
}


zaf::Size CompoundRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    zaf::Size result;
    result.width = bound_size.width;

    zaf::Size child_bound_size{};
    child_bound_size.width = result.width;
    child_bound_size.height = std::numeric_limits<float>::max();

    for (const auto& each_region : child_regions_) {

        auto child_size = each_region->CalculatePreferredSize(child_bound_size);
        result.height += child_size.height + each_region->Margin().Height();
    }

    return result;
}

}