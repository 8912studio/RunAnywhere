#pragma once

#include "help/markdown/element/element.h"

namespace ra::help::markdown::element {

enum class HeaderDepth {
    _1 = 1,
    _2 = 2,
    _3 = 3,
};

class HeaderElement : public Element {
public:
    HeaderElement(HeaderDepth depth, ElementList children);

    HeaderDepth Depth() const {
        return depth_;
    }

    bool IsEqualTo(const Element& other) const override;

private:
    HeaderDepth depth_{ HeaderDepth::_1 };
};

}