#include "utility/sql/data_type.h"

namespace ra::utility::sql {
namespace {

bool AreEqualCaseInsensitive(std::string_view string1, std::string_view string2) {

    if (string1.length() != string2.length()) {
        return false;
    }

    return std::equal(string1.begin(), string1.end(), string2.begin(), [](auto ch1, auto ch2) {
        return std::tolower(ch1) == std::tolower(ch2);
    });
}

}

std::string_view DataTypeTraits::ToString(DataType data_type) {
    switch (data_type) {
    case DataType::Integer:
        return "integer";
    case DataType::Float:
        return "float";
    case DataType::Text:
        return "text";
    case DataType::BLOB:
        return "blob";
    case DataType::Null:
    default:
        return "";
    }
}


DataType DataTypeTraits::FromString(std::string_view string) {

    if (AreEqualCaseInsensitive("integer", string)) {
        return DataType::Integer;
    }

    if (AreEqualCaseInsensitive("float", string)) {
        return DataType::Float;
    }

    if (AreEqualCaseInsensitive("text", string)) {
        return DataType::Text;
    }

    if (AreEqualCaseInsensitive("blob", string)) {
        return DataType::BLOB;
    }

    return DataType::Null;
}

}