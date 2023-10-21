#pragma once

#include <string>
#include "utility/markdown/element/element.h"

namespace ra::mod {

class CommandBrief {
public:
    CommandBrief(
        std::wstring keyword,
        std::shared_ptr<help::markdown::element::Element> description);

    CommandBrief(std::wstring keyword, std::wstring description);

    const std::wstring& Keyword() const {
        return keyword_;
    }

    const std::shared_ptr<help::markdown::element::Element>& Description() const {
        return description_;
    }

private:
    std::wstring keyword_;
    std::shared_ptr<help::markdown::element::Element> description_;
};

}