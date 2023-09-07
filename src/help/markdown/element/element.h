#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element_type.h"

namespace ra::help::markdown::element {

class Element;
using ElementList = std::vector<std::shared_ptr<Element>>;

class Element : zaf::NonCopyableNonMovable {
public:
    //Construct a text element.
    explicit Element(std::wstring text);

    //Construct an element with children.
    Element(ElementType type, ElementList children);

    virtual ~Element() = default;

    ElementType Type() const {
        return type_;
    }

    bool IsTextElement() const {
        return type_ == ElementType::Text;
    }

    const std::wstring& Text() const {
        return std::get<std::wstring>(data_);
    }

    const ElementList& Children() const {
        return std::get<ElementList>(data_);
    }

    virtual bool IsEqualTo(const Element& other) const;

private:
    ElementType type_;
    std::variant<std::wstring, ElementList> data_;
};

}