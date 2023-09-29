#pragma once

#include <optional>
#include "help/markdown/parse/block_parser.h"

namespace ra::help::markdown::parse {

class BodyParser;

class ListItemParser : public BlockParser {
public:
    Status ParseOneLine(ParseContext& context) override;
    Result FinishCurrentElement() override;

protected:
    virtual bool ParseItemIdentity(ParseContext& context) = 0;

private:
    class State {
    public:
        State();
        ~State();

        std::unique_ptr<BodyParser> body_parser;
    };

private:
    bool ParseItemBodyLine(ParseContext& context);
    bool InnerParseItemBodyLine(ParseContext& context);

private:
    std::optional<State> state_;
};

}