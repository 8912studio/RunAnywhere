#pragma once

#include <zaf/control/text_box.h>
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/simple_block_region.h"
#include "utility/markdown/render/style_config.h"

namespace ra::utility::markdown::render {

class ParagraphRegion : public SimpleBlockRegion {
public:
    static std::shared_ptr<ParagraphRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

private:
    ParagraphRegion() = default;

    void InitializeStyle(const StyleConfig& style_config);
};

}