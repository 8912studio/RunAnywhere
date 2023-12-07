#include "module/tool/json/json_formatter.h"
#include <zaf/base/error/check.h>
#include <zaf/base/string/encoding_conversion.h>

using namespace ra::utility::markdown::render;

namespace ra::mod::tool::json {
namespace {

std::wstring GetIdent(std::size_t deep) {
    return std::wstring(4 * deep, L' ');
}

}

JSONFormatter::JSONFormatter() {

    default_style_.font.family_name = L"Consolas";
    default_style_.font.size = 16;
    default_style_.text_color = zaf::Color::Gray();

    key_style_ = default_style_;
    key_style_.text_color = zaf::Color::Black();

    string_style_ = default_style_;
    string_style_.text_color = zaf::Color::FromRGB(0xAA3322);

    number_style_ = default_style_;
    number_style_.text_color = zaf::Color::Green();

    keyword_style_ = default_style_;
    keyword_style_.text_color = zaf::Color::FromRGB(0x0055FF);
}


StyledText JSONFormatter::Format(const boost::json::value& value) const {

    StyledText result;
    FormatValue(value, 0, result);

    result.AddStyleToPendingText(default_style_);
    return result;
}


void JSONFormatter::FormatValue(
    const boost::json::value& value,
    std::size_t deep, 
    StyledText& styled_text) const {

    switch (value.kind()) {
    case boost::json::kind::null:
        styled_text.Append(L"null", keyword_style_);
        break;
    case boost::json::kind::bool_:
        styled_text.Append(value.get_bool() ? L"true" : L"false", keyword_style_);
        break;
    case boost::json::kind::int64:
        styled_text.Append(std::to_wstring(value.get_int64()), number_style_);
        break;
    case boost::json::kind::uint64:
        styled_text.Append(std::to_wstring(value.get_uint64()), number_style_);
        break;
    case boost::json::kind::double_:
        styled_text.Append(std::to_wstring(value.get_double()), number_style_);
        break;
    case boost::json::kind::string:
        styled_text.Append(L"\"");
        styled_text.Append(zaf::FromUTF8String(value.get_string()));
        styled_text.Append(L"\"");
        styled_text.AddStyleToPendingText(string_style_);
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

    styled_text.Append(L"[\n");

    auto new_deep = deep + 1;
    auto ident = GetIdent(new_deep);

    for (auto iterator = array_value.begin(); iterator != array_value.end(); ++iterator) {

        if (iterator != array_value.begin()) {
            styled_text.Append(L",\n");
        }

        styled_text.Append(ident);
        styled_text.AddStyleToPendingText(default_style_);
        FormatValue(*iterator, new_deep, styled_text);
    }

    styled_text.Append(L"\n");
    styled_text.Append(GetIdent(deep));
    styled_text.Append(L"]");
    styled_text.AddStyleToPendingText(default_style_);
}


void JSONFormatter::FormatObject(
    const boost::json::object& object,
    std::size_t deep, 
    StyledText& styled_text) const {

    styled_text.Append(L"{\n");

    auto new_deep = deep + 1;
    auto ident = GetIdent(new_deep);

    for (auto iterator = object.begin(); iterator != object.end(); ++iterator) {

        if (iterator != object.begin()) {
            styled_text.Append(L",\n");
        }

        styled_text.Append(ident);
        styled_text.AddStyleToPendingText(default_style_);

        styled_text.Append(L"\"");
        styled_text.Append(zaf::FromUTF8String(iterator->key()));
        styled_text.Append(L"\"");
        styled_text.AddStyleToPendingText(key_style_);

        styled_text.Append(L": ", default_style_);
        FormatValue(iterator->value(), new_deep, styled_text);
    }

    styled_text.Append(L"\n");
    styled_text.Append(GetIdent(deep));
    styled_text.Append(L"}");
    styled_text.AddStyleToPendingText(default_style_);
}

}