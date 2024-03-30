#include "module/tool/json/json_formatter.h"
#include <format>
#include <zaf/base/error/check.h>
#include <zaf/base/string/encoding_conversion.h>

using namespace zaf;
using namespace zaf::textual;

namespace ra::mod::tool::json {
namespace {

std::wstring GetIdent(std::size_t deep) {
    return std::wstring(4 * deep, L' ');
}

}

JSONFormatter::JSONFormatter() {

    key_style_.SetTextColor(Color::Black());
    string_style_.SetTextColor(Color::FromRGB(0xAA3322));
    number_style_.SetTextColor(Color::Green());
    keyword_style_.SetTextColor(Color::FromRGB(0x0055FF));
}


StyledText JSONFormatter::Format(const boost::json::value& value) const {

    StyledText result;
    result.SetDefaultFont(Font{ L"Consolas", 16 });
    result.SetDefaultTextColor(Color::Gray());

    FormatValue(value, 0, result);
    return result;
}


void JSONFormatter::FormatValue(
    const boost::json::value& value,
    std::size_t deep, 
    StyledText& styled_text) const {

    switch (value.kind()) {
    case boost::json::kind::null: 
        styled_text.AppendText(L"null", keyword_style_);
        break;
    case boost::json::kind::bool_: 
        styled_text.AppendText(value.get_bool() ? L"true" : L"false", keyword_style_);
        break;
    case boost::json::kind::int64: 
        styled_text.AppendText(std::to_wstring(value.get_int64()), number_style_);
        break;
    case boost::json::kind::uint64: 
        styled_text.AppendText(std::to_wstring(value.get_uint64()), number_style_);
        break;
    case boost::json::kind::double_: 
        styled_text.AppendText(std::to_wstring(value.get_double()), number_style_);
        break;
    case boost::json::kind::string: 
        styled_text.AppendText(
            std::format(L"\"{}\"", FromUTF8String(value.get_string())), 
            string_style_);
        break;
    case boost::json::kind::array:
        FormatArray(value.get_array(), deep, styled_text);
        break;
    case boost::json::kind::object:
        FormatObject(value.get_object(), deep, styled_text);
        break;
    default:
        ZAF_NOT_REACHED();
    }
}


void JSONFormatter::FormatArray(
    const boost::json::array& array_value,
    std::size_t deep,
    StyledText& styled_text) const {

    styled_text.AppendText(L"[\n");

    auto new_deep = deep + 1;
    auto ident = GetIdent(new_deep);

    for (auto iterator = array_value.begin(); iterator != array_value.end(); ++iterator) {

        if (iterator != array_value.begin()) {
            styled_text.AppendText(L",\n");
        }

        styled_text.AppendText(ident);
        FormatValue(*iterator, new_deep, styled_text);
    }

    styled_text.AppendText(std::format(L"\n{}]", GetIdent(deep)));
}


void JSONFormatter::FormatObject(
    const boost::json::object& object,
    std::size_t deep, 
    StyledText& styled_text) const {

    styled_text.AppendText(L"{\n");

    auto new_deep = deep + 1;
    auto ident = GetIdent(new_deep);

    for (auto iterator = object.begin(); iterator != object.end(); ++iterator) {

        if (iterator != object.begin()) {
            styled_text.AppendText(L",\n");
        }

        styled_text.AppendText(ident);

        styled_text.AppendText(
            std::format(L"\"{}\"", FromUTF8String(iterator->key())),
            key_style_);

        styled_text.AppendText(L": ");
        FormatValue(iterator->value(), new_deep, styled_text);
    }

    styled_text.AppendText(std::format(L"\n{}}}", GetIdent(deep)));
}

}