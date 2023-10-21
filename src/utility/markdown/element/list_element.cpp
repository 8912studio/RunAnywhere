#include "utility/markdown/element/list_element.h"
#include <zaf/base/error/check.h>

namespace ra::help::markdown::element {

ListElement::ListElement(ElementType type, ElementList children, ListItemStyle item_style) :
    Element(type, std::move(children)),
    item_style_(item_style) {

}


bool ListElement::IsEqualTo(const Element& other) const {

    if (!__super::IsEqualTo(other)) {
        return false;
    }

    return this->ItemStyle() == dynamic_cast<const ListElement&>(other).ItemStyle();
}


OrderedListElement::OrderedListElement(ListItemStyle item_style, ElementList children) :
    ListElement(ElementType::OrderedList, std::move(children), item_style) {

}


UnorderedListElement::UnorderedListElement(ListItemStyle item_style, ElementList children) :
    ListElement(ElementType::UnorderedList, std::move(children), item_style) {

}

}