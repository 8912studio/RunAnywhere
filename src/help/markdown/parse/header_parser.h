#pragma once

#include "help/markdown/parse/block_parser.h"

namespace ra::help::markdown::parse {

class HeaderParser : public BlockParser {
public:
    Status ParseOneLine(ParseContext& context) override;
    std::shared_ptr<element::Element> FinishCurrentElement() override;

private:
    class State {
    public:
        std::size_t hash_count{};
        std::wstring content;
    };

private:
    std::optional<State> state_;
};

}