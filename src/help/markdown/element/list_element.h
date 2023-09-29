#pragma once

#include "help/markdown/element/element.h"

namespace ra::help::markdown::element {

enum class ListItemStyle {
    Lines,
    Blocks,
};

class ListElement : public Element {
public:
    bool IsEqualTo(const Element& other) const override;

    ListItemStyle ItemStyle() const {
        return item_style_;
    }

protected:
    ListElement(ElementType type, ElementList children, ListItemStyle item_style);

private:
    ListItemStyle item_style_{ ListItemStyle::Lines };
};


class OrderedListElement : public ListElement {
public:
    OrderedListElement(ListItemStyle item_style, ElementList children);
};


class UnorderedListElement : public ListElement {
public:
    UnorderedListElement(ListItemStyle item_style, ElementList children);
};

}