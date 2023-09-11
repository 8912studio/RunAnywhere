#pragma once

#include "help/markdown/element/element.h"
#include "help/markdown/element/header_element.h"

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


inline std::shared_ptr<Element> MakeBold(ElementList children) {
    return std::make_shared<Element>(ElementType::Bold, std::move(children));
}


inline std::shared_ptr<Element> MakeInlineCode(std::wstring text) {
    return std::make_shared<Element>(ElementType::InlineCode, ElementList{ 
        std::make_shared<Element>(std::move(text)), 
    });
}


inline std::shared_ptr<Element> MakeHeader(HeaderDepth depth, ElementList children) {
    return std::make_shared<HeaderElement>(depth, std::move(children));
}

}