#include "utility/markdown/element/code_block_element.h"

namespace ra::utility::markdown::element {

CodeBlockElement::CodeBlockElement(std::wstring language, std::wstring code) : 
    Element(ElementType::CodeBlock, { std::make_shared<Element>(std::move(code)) }),
    language_(std::move(language)) {

}


bool CodeBlockElement::IsEqualTo(const Element& other) const {
    
    if (!__super::IsEqualTo(other)) {
        return false;
    }

    return this->Language() == dynamic_cast<const CodeBlockElement&>(other).Language();
}

}