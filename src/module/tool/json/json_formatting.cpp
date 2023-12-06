#include "module/tool/json/json_formatting.h"
#include <sstream>
#include <zaf/base/error/check.h>

namespace ra::mod::tool::json {
namespace {

void FormatToStream(const boost::json::value& value, std::size_t deep, std::ostringstream& stream);


std::string GetIdent(std::size_t deep) {
    return std::string(4 * deep, ' ');
}


void FormatArrayToStream(
    const boost::json::array& array_value,
    std::size_t deep,
    std::ostringstream& stream) {

    stream << "[\n";

    auto new_deep = deep + 1;
    auto ident = GetIdent(new_deep);

    for (auto iterator = array_value.begin(); iterator != array_value.end(); ++iterator) {

        if (iterator != array_value.begin()) {
            stream << ",\n";
        }

        stream << ident;
        FormatToStream(*iterator, new_deep, stream);
    }

    stream << '\n' << GetIdent(deep) << ']';
}


void FormatObjectToStream(
    const boost::json::object& object,
    std::size_t deep,
    std::ostringstream& stream) {

    stream << "{\n";

    auto new_deep = deep + 1;
    auto ident = GetIdent(new_deep);

    for (auto iterator = object.begin(); iterator != object.end(); ++iterator) {

        if (iterator != object.begin()) {
            stream << ",\n";
        }

        stream << ident;
        stream << '"' << iterator->key() << "\": ";
        FormatToStream(iterator->value(), new_deep, stream);
    }

    stream << '\n' << GetIdent(deep) << '}';
}


void FormatToStream(
    const boost::json::value& value, 
    std::size_t deep,
    std::ostringstream& stream) {

    switch (value.kind()) {
    case boost::json::kind::null:
        stream << "null";
        break;
    case boost::json::kind::bool_:
        stream << (value.get_bool() ? "true" : "false");
        break;
    case boost::json::kind::int64:
        stream << value.get_int64();
        break;
    case boost::json::kind::uint64:
        stream << value.get_uint64();
        break;
    case boost::json::kind::double_:
        stream << value.get_double();
        break;
    case boost::json::kind::string:
        stream << value.get_string();
        break;
    case boost::json::kind::array:
        FormatArrayToStream(value.get_array(), deep, stream);
        break;
    case boost::json::kind::object:
        FormatObjectToStream(value.get_object(), deep, stream);
        break;
    default:
        ZAF_NOT_REACHED();
    }
}

}

std::string FormatJSONValue(const boost::json::value& value) {

    std::ostringstream stream;
    FormatToStream(value, 0, stream);
    return stream.str();
}

}