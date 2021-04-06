#include "module/calculator/parse/literal_parser.h"
#include "module/calculator/parse/parse_context.h"

namespace calculator {

ParseStatus LiteralParser::Parse(ParseContext& context, ParseResult& parse_result) {

    for (std::size_t index = 0; index < literal_.length(); ++index) {

        if (context.GetCurrentChar() != literal_[index]) {
            if (index == 0) {
                return ParseStatus::Mismatched;
            }
            else {
                return ParseStatus::Error;
            }
        }

        if (!context.Forward()) {
            return ParseStatus::Error;
        }
    }

    return ParseStatus::Ok;
}

}