#pragma once

#include "utility/markdown/element/element.h"

namespace ra::utility::markdown::element {

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
    OrderedListElement(ListItemStyle item_style, std::size_t first_number, ElementList children);

    std::size_t FirstNumber() const {
        return first_number_;
    }

    bool IsEqualTo(const Element& other) const override;

private:
    std::size_t first_number_{};
};


class UnorderedListElement : public ListElement {
public:
    UnorderedListElement(ListItemStyle item_style, ElementList children);
};

}