#include "utility/markdown/element/list_element.h"
#include <zaf/base/error/check.h>

namespace ra::utility::markdown::element {

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


OrderedListElement::OrderedListElement(
    ListItemStyle item_style,
    std::size_t first_number, 
    ElementList children) 
    :
    ListElement(ElementType::OrderedList, std::move(children), item_style),
    first_number_(first_number) {

}


bool OrderedListElement::IsEqualTo(const Element& other) const {

    if (!__super::IsEqualTo(other)) {
        return false;
    }

    return this->FirstNumber() == dynamic_cast<const OrderedListElement&>(other).FirstNumber();
}


UnorderedListElement::UnorderedListElement(ListItemStyle item_style, ElementList children) :
    ListElement(ElementType::UnorderedList, std::move(children), item_style) {

}

}