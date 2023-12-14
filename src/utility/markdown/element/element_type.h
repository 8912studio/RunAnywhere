#pragma once

namespace ra::utility::markdown::element {

enum class ElementType {

    Root,

    Paragraph,

    //Block elements
    Header,
    CodeBlock,
    OrderedList,
    UnorderedList,
    ListItem,

    //Span elements
    Italic,
    Bold,
    InlineCode,

    Text,
};


inline bool IsBlockElementType(ElementType type) {
    return
        type == ElementType::Header ||
        type == ElementType::CodeBlock ||
        type == ElementType::OrderedList ||
        type == ElementType::UnorderedList;
}

}