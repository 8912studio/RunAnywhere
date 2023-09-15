#pragma once

#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"
#include "help/markdown/parse/parse_context.h"

namespace ra::help::markdown::parse {

/**
Base class of block element parsers.
*/
class BlockParser : zaf::NonCopyableNonMovable {
public:
    enum class Status {
        Failed,
        Continue,
        Finished,
    };

public:
    virtual ~BlockParser() = default;

    virtual Status ParseOneLine(ParseContext& context) = 0;
    virtual std::shared_ptr<element::Element> FinishCurrentElement() = 0;
};

}