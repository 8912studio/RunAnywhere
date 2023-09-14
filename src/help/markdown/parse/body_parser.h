#pragma once

#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"
#include "help/markdown/parse/parse_context.h"

namespace ra::help::markdown::parse {

class BodyParser : zaf::NonCopyableNonMovable {
public:
    static BodyParser* Instance();

public:
    element::ElementList Parse(ParseContext& context);

private:
    static void ParseEmptyLines(ParseContext& context);
    static bool ParseEmptyLine(ParseContext& context);

private:
    BodyParser() = default;
};

}