#pragma once

#include <zaf/control/text_box.h>
#include "help/markdown/element/element.h"
#include "help/markdown/render/simple_block_region.h"
#include "help/markdown/render/style_config.h"
#include "help/markdown/render/text_style.h"

namespace ra::help::markdown::render {

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