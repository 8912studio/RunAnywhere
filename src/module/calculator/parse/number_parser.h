#pragma once

#include "module/calculator/parse/non_terminal_parser.h"

namespace ra::module::calculator {

class NumberParser : public NonTerminalParser {
public:
    enum class NumberType {
        Decimal,
        Hex,
        Binary,
        Octal,
    };

    //A default number parser that supports all types.
    static NumberParser* Default();

    //Create a number parser that supports only specified number types.
    static std::unique_ptr<NumberParser> Create(
        const std::vector<NumberType>& supported_number_types);

protected:
    void InitializeParsers() override;

private:
    NumberParser(const std::vector<NumberType>& supported_number_types);

private:
    std::vector<NumberType> supported_number_types_;
};

}