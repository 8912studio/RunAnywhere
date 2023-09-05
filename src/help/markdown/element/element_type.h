#pragma once

namespace ra::help::markdown::element {

enum class ElementType {

    Root,

    //Block elements
    Paragraph,
    Header,
    CodeBlock,

    //Span elements
    InlineCode,

    Text,
};

}