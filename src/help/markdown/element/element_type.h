#pragma once

namespace ra::help::markdown::element {

enum class ElementType {

    Root,

    //Block elements
    Paragraph,
    Header,
    CodeBlock,
    OrderedList,
    UnorderedList,
    ListItem,

    //Span elements
    Italics,
    Bold,
    InlineCode,

    Text,
};

}