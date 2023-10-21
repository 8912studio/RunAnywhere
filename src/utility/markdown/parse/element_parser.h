#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/markdown/element/element.h"
#include "utility/markdown/parse/parse_context.h"
#include "module/calculator/parse/parse_status.h"

namespace ra::utility::markdown::parse {

class ElementParser : zaf::NonCopyableNonMovable {
public:
    virtual ~ElementParser() = default;

    virtual std::shared_ptr<element::Element> Parse(ParseContext& context) = 0;
};

}