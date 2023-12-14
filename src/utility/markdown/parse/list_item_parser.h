#pragma once

#include <optional>
#include "utility/markdown/parse/block_parser.h"

namespace ra::utility::markdown::parse {

class BodyParser;

class ListItemParser : public BlockParser {
public:
    Status ParseOneLine(ParseContext& context) override;
    Result FinishCurrentElement() override;

    std::size_t IndentCharCount() const {
        return state_->indent_char_count;
    }

protected:
    virtual bool ParseItemIdentity(ParseContext& context) = 0;

private:
    class State {
    public:
        State();
        ~State();

        std::size_t indent_char_count{};
        std::unique_ptr<BodyParser> body_parser;
    };

private:
    bool ParseItemBodyLine(ParseContext& context);
    bool InnerParseItemBodyLine(ParseContext& context);
    bool IsAtBlockHead(ParseContext& context) const;

private:
    std::optional<State> state_;
};

}