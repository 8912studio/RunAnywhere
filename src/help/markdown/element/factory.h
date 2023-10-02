#pragma once

#include "help/markdown/element/element.h"
#include "help/markdown/element/header_element.h"
#include "help/markdown/element/list_element.h"

namespace ra::help::markdown::element {

inline std::shared_ptr<Element> MakeRoot(ElementList children) {
    return std::make_shared<Element>(ElementType::Root, std::move(children));
}


inline std::shared_ptr<Element> MakeText(std::wstring text) {
    return std::make_shared<Element>(std::move(text));
}


inline std::shared_ptr<Element> MakeParagraph(ElementList children) {
    return std::make_shared<Element>(ElementType::Paragraph, std::move(children));
}

inline std::shared_ptr<Element> MakeParagraph(std::wstring text) {
    return std::make_shared<Element>(ElementType::Paragraph, ElementList{
        std::make_shared<Element>(std::move(text))
    });
}


inline std::shared_ptr<Element> MakeItalic(ElementList children) {
    return std::make_shared<Element>(ElementType::Italic, std::move(children));
}

inline std::shared_ptr<Element> MakeItalic(std::wstring text) {
    return std::make_shared<Element>(ElementType::Italic, ElementList{ 
        MakeText(std::move(text)) 
    });
}


inline std::shared_ptr<Element> MakeBold(ElementList children) {
    return std::make_shared<Element>(ElementType::Bold, std::move(children));
}

inline std::shared_ptr<Element> MakeBold(std::wstring text) {
    return std::make_shared<Element>(ElementType::Bold, ElementList{
        std::make_shared<Element>(std::move(text))
    });
}


inline std::shared_ptr<Element> MakeInlineCode(std::wstring text) {
    return std::make_shared<Element>(ElementType::InlineCode, ElementList{ 
        std::make_shared<Element>(std::move(text)), 
    });
}


inline std::shared_ptr<Element> MakeCodeBlock(std::wstring text) {
    return std::make_shared<Element>(ElementType::CodeBlock, ElementList{
        std::make_shared<Element>(std::move(text)),
    });
}


inline std::shared_ptr<Element> MakeHeader(HeaderDepth depth, ElementList children) {
    return std::make_shared<HeaderElement>(depth, std::move(children));
}

inline std::shared_ptr<Element> MakeHeader(HeaderDepth depth, std::wstring text) {
    return std::make_shared<HeaderElement>(depth, ElementList{
        std::make_shared<Element>(std::move(text)),
    });
}


inline std::shared_ptr<Element> MakeListItem(ElementList children) {
    return std::make_shared<Element>(ElementType::ListItem, std::move(children));
}


inline std::shared_ptr<Element> MakeUnorderedList(ListItemStyle item_type, ElementList children) {
    return std::make_shared<UnorderedListElement>(item_type, std::move(children));
}


inline std::shared_ptr<Element> MakeOrderedList(ListItemStyle item_type, ElementList children) {
    return std::make_shared<OrderedListElement>(item_type, std::move(children));
}

}