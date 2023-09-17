#pragma once

#include <optional>
#include "help/markdown/parse/block_parser.h"
#include "help/markdown/parse/unordered_list_item_parser.h"

namespace ra::help::markdown::parse {

class UnorderedListParser : public BlockParser {
public:
    Status ParseOneLine(ParseContext& context) override;
    std::shared_ptr<element::Element> FinishCurrentElement() override;

private:
    class State {
    public:
        wchar_t identity_char{};
        std::unique_ptr<UnorderedListItemParser> current_item_parser;
        element::ElementList parsed_items;
    };

private:
    bool ParseFirstLine(ParseContext& context);
    bool ParseNonFirstLine(ParseContext& context);

private:
    std::optional<State> state_;
};

}