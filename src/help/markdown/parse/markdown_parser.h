#pragma once

#include <optional>
#include <string_view>
#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"

namespace ra::help::markdown::parse {

class MarkdownParser : zaf::NonCopyableNonMovable {
public:
    static MarkdownParser* Instance();

public:
    std::shared_ptr<element::Element> Parse(std::wstring_view input);

private:
    MarkdownParser() = default;
};

}