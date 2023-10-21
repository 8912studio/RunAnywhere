#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/markdown/element/element.h"
#include "utility/markdown/parse/empty_line_info.h"
#include "utility/markdown/parse/parse_context.h"

namespace ra::utility::markdown::parse {

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

    struct Result {

        Result() = default;

        Result(std::shared_ptr<element::Element> element) : element(std::move(element)) {

        }

        std::shared_ptr<element::Element> element;
        EmptyLineInfo empty_line_info;
    };

public:
    virtual ~BlockParser() = default;

    virtual Status ParseOneLine(ParseContext& context) = 0;
    virtual Result FinishCurrentElement() = 0;
};

}