#pragma once

#include <optional>
#include <string_view>
#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"
#include "help/markdown/parse/parse_context.h"

namespace ra::help::markdown::parse {

class MarkdownParser : zaf::NonCopyableNonMovable {
public:
    static MarkdownParser* Instance();

public:
    std::shared_ptr<element::Element> Parse(std::wstring_view input);

private:
    MarkdownParser() = default;

    void ParseEmptyLines(ParseContext& context) const;
    bool ParseEmptyLine(ParseContext& context) const;
};

}