#pragma once

#include "utility/markdown/parse/emphasis_parser.h"

namespace ra::help::markdown::parse {

class ItalicParser : public EmphasisParser {
public:
    static ElementParser* Instance();

private:
    static Options GetOptions();

private:
    ItalicParser();
};

}