#include "module/calculator/parse/common_option_set_parser.h"
#include <cctype>
#include "module/calculator/parse/terminal_parser.h"

namespace ra::module::calculator {
namespace {

class BaseOptionParser : public TerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& result) override {

        auto& modifier = result.GetMutableModifier();

        auto reader = context.BeginRead();
        wchar_t current_char = reader.GetChar();
        wchar_t lower_char = std::tolower(current_char);
        if (lower_char == L'b') {
            modifier.base = 2;
        }
        else if (lower_char == L'o') {
            modifier.base = 8;
        }
        else if (lower_char == L'd') {
            modifier.base = 10;
        }
        else if (lower_char == L'x') {
            modifier.base = 16;
        }
        else {
            return ParseStatus::Mismatched;
        }

        if (current_char != lower_char) {
            modifier.use_upper_case = true;
        }

        reader.Forward();
        return ParseStatus::Ok;
    }
};


class LengthOptionParser : public TerminalParser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& result) override {

        auto& modifier = result.GetMutableModifier();

        auto reader = context.BeginRead();
        auto ch = std::tolower(reader.GetChar());
        if (ch == L'b') {
            modifier.bit_length = 8;
        }
        else if (ch == L'w') {
            modifier.bit_length = 16;
        }
        else if (ch == L'd') {
            modifier.bit_length = 32;
        }
        else if (ch == L'q') {
            modifier.bit_length = 64;
        }
        else {
            return ParseStatus::Ok;
        }

        reader.Forward();
        return ParseStatus::Ok;
    }
};

}

CommonOptionSetParser* CommonOptionSetParser::Instance() {

    static CommonOptionSetParser instance;
    return &instance;
}


void CommonOptionSetParser::InitializeParsers() {

    static BaseOptionParser base_option_parser;
    static LengthOptionParser length_option_parser;
    AddParsers({ 
        &base_option_parser, 
        &length_option_parser
    });

    AddParsers({});
}

}