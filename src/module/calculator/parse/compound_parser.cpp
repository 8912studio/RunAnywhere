#include "module/calculator/parse/compound_parser.h"
#include <cctype>
#include "module/calculator/parse/parse_context.h"

namespace ra::mod::calculator {

ParseStatus CompoundParser::Parse(ParseContext& context, ParseResult& result) {

    for (const auto& each_parser : parsers_) {

        auto status = each_parser->Parse(context, result);
        if (status == ParseStatus::Error) {
            return status;
        }

        if (status == ParseStatus::Mismatched) {
            if (each_parser == parsers_.front()) {
                return status;
            }
            else {
                return ParseStatus::Error;
            }
        }

        context.SkipWhiteSpaces();
    }

    return ParseStatus::Ok;
}

}