#include "utility/markdown/element/element.h"
#include <zaf/base/error/contract_error.h>
#include <zaf/base/range.h>
#include "utility/markdown/element/header_element.h"

namespace ra::utility::markdown::element {

Element::Element(std::wstring text) : type_(ElementType::Text), data_(std::move(text)) {

}


Element::Element(ElementType type, ElementList children) : 
    type_(type), 
    data_(std::move(children)) {

    ZAF_EXPECT(type_ != ElementType::Text);
}


bool Element::IsEqualTo(const Element& other) const {

    if (this == &other) {
        return true;
    }

    if (this->Type() != other.Type()) {
        return false;
    }

    if (this->IsTextElement()) {
        return this->Text() == other.Text();
    }

    const auto& this_children = this->Children();
    const auto& other_children = other.Children();
    if (this_children.size() != other_children.size()) {
        return false;
    }

    for (auto index : zaf::Range(0, this_children.size())) {

        if (!this_children[index]->IsEqualTo(*other_children[index])) {
            return false;
        }
    }

    return true;
}

}