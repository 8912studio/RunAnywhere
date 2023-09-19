#pragma once

#include "help/markdown/render/region.h"

namespace ra::help::markdown::render {

class RootRegion : public Region {
public:
    void Initialize(const element::Element& element, const TextStyle& text_style) override;
};

}