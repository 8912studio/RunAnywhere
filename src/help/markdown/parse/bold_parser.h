#pragma once

#include "help/markdown/parse/emphasis_parser.h"

namespace ra::help::markdown::parse {

class BoldParser : public EmphasisParser {
public:
    static ElementParser* Instance();

private:
    static Options GetOptions();

private:
    BoldParser();
};

}