#pragma once

#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"
#include "help/markdown/render/text_style.h"

namespace ra::help::markdown::render {

class Region : zaf::NonCopyableNonMovable {
public:
    virtual ~Region() = default;

    virtual void Initialize(const element::Element& element, const TextStyle& text_style) = 0;
};

}