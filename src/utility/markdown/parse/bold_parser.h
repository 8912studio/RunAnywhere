#pragma once

#include "utility/markdown/parse/emphasis_parser.h"

namespace ra::utility::markdown::parse {

class BoldParser : public EmphasisParser {
public:
    static ElementParser* Instance();

private:
    static Options GetOptions();

private:
    BoldParser();
};

}