#pragma once

#include <vector>
#include <zaf/base/range.h>
#include "help/markdown/style.h"

namespace ra::help::markdown {

class StyleSpan {
public:
    zaf::Range range;
    Style style;
    std::vector<StyleSpan> interior_spans;
};

}