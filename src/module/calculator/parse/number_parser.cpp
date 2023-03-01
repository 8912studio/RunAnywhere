#include "module/calculator/parse/number_parser.h"
#include <zaf/base/container/utility/contain.h>
#include "module/calculator/parse/decimal_number_parser.h"
#include "module/calculator/parse/non_decimal_number_parser.h"
#include "module/calculator/parse/octal_number_parser.h"

namespace ra::module::calculator {

NumberParser* NumberParser::Default() {
    static NumberParser instance{{ 
        NumberType::Decimal, 
        NumberType::Hex,
        NumberType::Octal,
        NumberType::Binary,
    }};
    return &instance;
}


std::unique_ptr<NumberParser> NumberParser::Create(
        const std::vector<NumberParser::NumberType>& supported_number_types) {

    return std::unique_ptr<NumberParser>(new NumberParser{ supported_number_types });
}


NumberParser::NumberParser(const std::vector<NumberType>& supported_number_types) :
    supported_number_types_(supported_number_types) {

}


void NumberParser::InitializeParsers() {

    //Note that the order of parser is mandatory.

    if (zaf::Contain(supported_number_types_, NumberType::Binary)) {
        AddParsers({ NonDecimalNumberParser::Binary() });
    }

    if (zaf::Contain(supported_number_types_, NumberType::Hex)) {
        AddParsers({ NonDecimalNumberParser::Hex() });
    }

    if (zaf::Contain(supported_number_types_, NumberType::Octal)) {
        AddParsers({ OctalNumberParser::Instance() });
    }

    if (zaf::Contain(supported_number_types_, NumberType::Decimal)) {
        AddParsers({ DecimalNumberParser::Instance() });
    }
}

}