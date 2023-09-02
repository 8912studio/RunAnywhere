#pragma once

#include <zaf/base/non_copyable.h>
#include "module/calculator/parse/parse_context.h"
#include "module/calculator/parse/parse_status.h"
#include "help/markdown/styled_string_builder.h"

namespace ra::help::markdown {

using ParseContext = mod::calculator::ParseContext;
using ParseStatus = mod::calculator::ParseStatus;

class StyleParser : zaf::NonCopyableNonMovable {
public:
    virtual ~StyleParser() = default;

    virtual ParseStatus Parse(ParseContext& context, StyledStringBuilder& style_builder) = 0;
};

}