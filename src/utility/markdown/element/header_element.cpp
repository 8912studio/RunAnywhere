#include "utility/markdown/element/header_element.h"

namespace ra::utility::markdown::element {

HeaderElement::HeaderElement(HeaderDepth depth, ElementList children) :
    Element(ElementType::Header, std::move(children)),
    depth_(depth) {

}


bool HeaderElement::IsEqualTo(const Element& other) const {

    if (!__super::IsEqualTo(other)) {
        return false;
    }

    return this->Depth() == dynamic_cast<const HeaderElement&>(other).Depth();
}

}