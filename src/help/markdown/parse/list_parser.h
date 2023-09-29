#pragma once

#include "help/markdown/element/list_element.h"
#include "help/markdown/parse/block_parser.h"
#include "help/markdown/parse/list_item_parser.h"

namespace ra::help::markdown::parse {

class ListParser : public BlockParser {
public:
    Status ParseOneLine(ParseContext& context) override;
    Result FinishCurrentElement() override;

protected:
    virtual std::unique_ptr<ListItemParser> CreateFirstItemParser(ParseContext& context) = 0;
    virtual std::unique_ptr<ListItemParser> CreateNonFirstItemParser(ParseContext& context) = 0;
    virtual std::shared_ptr<element::Element> CreateListElement(
        element::ListItemStyle item_style,
        element::ElementList list_items) = 0;

private:
    class State {
    public:
        std::unique_ptr<ListItemParser> current_item_parser;
        element::ElementList parsed_items;
        EmptyLineInfo empty_line_info;
    };

private:
    bool ParseFirstLine(ParseContext& context);
    bool ParseNonFirstLine(ParseContext& context);
    void FinishCurrentItem();

private:
    std::optional<State> state_;
};

}