#pragma once

#include <string_view>

namespace ra::utility::sql {

enum class Operator {
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    And,
    Or,
};

std::string_view ConvertOperatorTagToString(Operator op);

}