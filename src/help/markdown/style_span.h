#pragma once

#include <vector>
#include <zaf/base/range.h>
#include "help/markdown/style.h"

namespace ra::help::markdown {

class StyleSpan {
public:
    StyleSpan() = default;

    StyleSpan(std::size_t index, std::size_t length, StyleType style_type) :
        range(index, length) {

        style.type = style_type;
    }

    StyleSpan(
        std::size_t index,
        std::size_t length,
        StyleType style_type,
        std::vector<StyleSpan> interior_spans) 
        :
        range(index, length),
        interior_spans(std::move(interior_spans)) {

        style.type = style_type;
    }

    zaf::Range range;
    Style style;
    std::vector<StyleSpan> interior_spans;

    friend bool operator==(const StyleSpan& span1, const StyleSpan& span2) {
        return 
            span1.range == span2.range &&
            span1.style == span2.style &&
            span1.interior_spans == span2.interior_spans;
    }
};

}