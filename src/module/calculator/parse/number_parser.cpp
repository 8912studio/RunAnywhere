#include "module/calculator/parse/number_parser.h"
#include "module/calculator/parse/decimal_number_parser.h"
#include "module/calculator/parse/non_decimal_number_parser.h"
#include "module/calculator/parse/octal_number_parser.h"

namespace calculator {

NumberParser* NumberParser::Instance() {

    static NumberParser instance;
    return &instance;
}


void NumberParser::InitializeParsers() {

    AddParsers({ NonDecimalNumberParser::Binary() });

    AddParsers({ NonDecimalNumberParser::Hex() });

    AddParsers({ OctalNumberParser::Instance() });

    AddParsers({ DecimalNumberParser::Instance() });
}

}