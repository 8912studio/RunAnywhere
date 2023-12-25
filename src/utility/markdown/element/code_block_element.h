#pragma once

#include "utility/markdown/element/element.h"

namespace ra::utility::markdown::element {

class CodeBlockElement : public Element {
public:
    CodeBlockElement(std::wstring language, std::wstring code);

    const std::wstring& Language() const {
        return language_;
    }

    bool IsEqualTo(const Element& other) const override;

private:
    std::wstring language_;
};

}