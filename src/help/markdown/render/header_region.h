#pragma once

#include <zaf/control/text_box.h>
#include "help/markdown/element/element.h"
#include "help/markdown/render/simple_block_region.h"
#include "help/markdown/render/style_config.h"
#include "help/markdown/render/text_style.h"

namespace ra::help::markdown::render {

class HeaderRegion : public SimpleBlockRegion {
public:
    static std::shared_ptr<HeaderRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

protected:
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    HeaderRegion() = default;
};

}