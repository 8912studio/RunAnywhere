#include "help/markdown/parse/ordered_list_item_parser.h"

namespace ra::help::markdown::parse {
namespace {

bool IsDigitChar(wchar_t ch) {
    return L'0' <= ch && ch <= L'9';
}

}

bool OrderedListItemParser::ParseItemIdentity(ParseContext& context) {

    auto transaction = context.BeginTransaction();
    context.SkipSpaces();

    std::size_t digit_count{};
    while (IsDigitChar(context.CurrentChar())) {
        ++digit_count;
        context.Forward();
    }

    if (digit_count == 0) {
        return false;
    }

    if (!context.ParseText(L". ")) {
        return false;
    }

    context.SkipSpaces();
    if (context.IsAtLineEnd()) {
        return false;
    }

    transaction.Commit();
    return true;
}

}