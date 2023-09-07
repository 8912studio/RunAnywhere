#pragma once

#include <optional>
#include <string_view>
#include "help/markdown/element/element.h"

namespace ra::help::markdown::parse {

class MarkdownParser {
public:
    std::shared_ptr<element::Element> Parse(std::wstring_view input);
};

}