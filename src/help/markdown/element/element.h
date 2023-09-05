#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <zaf/base/error/check.h>
#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element_type.h"

namespace ra::help::markdown::element {

class Element;
using ElementList = std::vector<std::shared_ptr<Element>>;

class Element : zaf::NonCopyableNonMovable {
public:
    //Construct a text element.
    explicit Element(std::wstring text) : type_(ElementType::Text), data_(std::move(text)) {

    }

    //Construct an element with children.
    Element(ElementType type, ElementList children) : type_(type), data_(std::move(children)) {
        ZAF_EXPECT(type_ != ElementType::Text);
    }

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

private:
    ElementType type_;
    std::variant<std::wstring, ElementList> data_;
};

}