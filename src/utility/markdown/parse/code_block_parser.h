#pragma once

#include <optional>
#include "utility/markdown/parse/block_parser.h"

namespace ra::help::markdown::parse {

class CodeBlockParser : public BlockParser {
public:
    Status ParseOneLine(ParseContext& context) override;
    Result FinishCurrentElement() override;

private:
    class State {
    public:
        std::size_t backquote_count{};
        std::wstring content;
    };

private:
    static bool ParseHeadingLine(ParseContext& context, std::size_t& backquote_count);

private:
    void ParseContentLine(ParseContext& context);
    bool ParseTailingLine(ParseContext& context);

private:
    std::optional<State> state_;
};

}