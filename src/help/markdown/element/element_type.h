#pragma once

namespace ra::help::markdown::element {

enum class ElementType {

    Root,

    //Block elements
    Paragraph,
    Header,
    CodeBlock,
    OrderedList,
    OrderedListItem,
    UnorderedList,
    UnorderedListItem,

    //Span elements
    Italics,
    Bold,
    InlineCode,

    Text,
};

}