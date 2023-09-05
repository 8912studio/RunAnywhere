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
    HeaderElement(HeaderDepth depth, ElementList children) : 
        Element(ElementType::Header, std::move(children)),
        depth_(depth) {

    }

    HeaderDepth Depth() const {
        return depth_;
    }

private:
    HeaderDepth depth_{ HeaderDepth::_1 };
};

}