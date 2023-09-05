#pragma once

#include "help/markdown/element/element.h"

namespace ra::help::markdown::element {

enum class HeaderDepth {
    _1,
    _2,
    _3,
};

class HeaderElement : public Element {
public:
    HeaderElement(HeaderDepth depth, ElementList children) : 
        Element(ElementType::Header, std::move(children)) {

    }

    HeaderDepth Depth() const {
        return depth_;
    }

private:
    HeaderDepth depth_{ HeaderDepth::_1 };
};

}