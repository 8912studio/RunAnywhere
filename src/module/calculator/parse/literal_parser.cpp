#include "module/calculator/parse/literal_parser.h"
#include "module/calculator/parse/parse_context.h"

namespace ra::module::calculator {

ParseStatus LiteralParser::Parse(ParseContext& context, ParseResult& parse_result) {

    auto reader = context.BeginRead();

    std::size_t index{};
    for (index = 0; index < literal_.length(); ++index) {

        auto ch = reader.GetChar();
        if (ch != literal_[index]) {
            break;
        }

        reader.Forward();
    }

    if (index == 0) {
        return ParseStatus::Mismatched;
    }

    if (index != literal_.length()) {
        return ParseStatus::Error;
    }

    return ParseStatus::Ok;
}

}