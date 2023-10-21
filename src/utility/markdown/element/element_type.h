#pragma once

namespace ra::utility::markdown::element {

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
    Italic,
    Bold,
    InlineCode,

    Text,
};

}