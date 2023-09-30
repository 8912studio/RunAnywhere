#pragma once

#include "help/markdown/parse/block_parser.h"

namespace ra::help::markdown::parse {

class HeaderParser : public BlockParser {
public:
    Status ParseOneLine(ParseContext& context) override;
    Result FinishCurrentElement() override;

private:
    class State {
    public:
        std::size_t hash_count{};
        element::ElementList children;
    };

private:
    static element::ElementList ParseChildren(ParseContext& context);

private:
    std::optional<State> state_;
};

}