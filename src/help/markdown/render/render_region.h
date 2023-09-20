#pragma once

#include <zaf/base/non_copyable.h>
#include <zaf/graphic/canvas.h>
#include <zaf/graphic/size.h>
#include "help/markdown/element/element.h"
#include "help/markdown/render/text_style.h"

namespace ra::help::markdown::render {

class RenderRegion : zaf::NonCopyableNonMovable {
public:
    virtual ~RenderRegion() = default;

    virtual void Resize(const zaf::Size& size) = 0;
    virtual void Paint(zaf::Canvas& canvas) = 0;
};

}