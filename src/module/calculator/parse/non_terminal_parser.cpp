#include "module/calculator/parse/non_terminal_parser.h"

namespace ra::module::calculator {

void NonTerminalParser::AddParsers(const std::vector<Parser*>& parsers) {

    auto compound_parser = std::make_shared<CompoundParser>();
    for (const auto& each_parser : parsers) {
        compound_parser->AddParser(each_parser);
    }

    compound_parsers_.push_back(compound_parser);
}


ParseStatus NonTerminalParser::Parse(ParseContext& context, ParseResult& result) {

    if (!has_initialize_parsers_) {
        InitializeParsers();
        has_initialize_parsers_ = true;
    }

    for (const auto& each_parser : compound_parsers_) {

        auto status = each_parser->Parse(context, result);
        if (status != ParseStatus::Mismatched) {
            return status;
        }
    }

    return ParseStatus::Mismatched;
}

}