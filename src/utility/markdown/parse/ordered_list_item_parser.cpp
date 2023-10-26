#include "utility/markdown/parse/ordered_list_item_parser.h"
#include <zaf/base/string/to_numeric.h>

namespace ra::utility::markdown::parse {
namespace {

bool IsDigitChar(wchar_t ch) {
    return L'0' <= ch && ch <= L'9';
}

}

bool OrderedListItemParser::ParseItemIdentity(ParseContext& context) {

    auto transaction = context.BeginTransaction();
    context.SkipSpaces();

    std::wstring number_string;
    while (IsDigitChar(context.CurrentChar())) {
        number_string += context.CurrentChar();
        context.Forward();
    }

    if (number_string.empty()) {
        return false;
    }

    if (!zaf::TryToNumeric<std::size_t>(number_string, number_)) {
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