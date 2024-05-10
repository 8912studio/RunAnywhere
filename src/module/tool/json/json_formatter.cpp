#include "module/tool/json/json_formatter.h"
#include <format>
#include <zaf/base/error/check.h>
#include <zaf/base/string/encoding_conversion.h>

using namespace zaf;
using namespace zaf::textual;

namespace ra::mod::tool::json {

void JSONFormatter::StartFormatting(const boost::json::value& value) {
    FormatValue(value);
}


void JSONFormatter::FormatValue(const boost::json::value& value) {

    switch (value.kind()) {
    case boost::json::kind::null: 
        OutputNull("null");
        break;
    case boost::json::kind::bool_: 
        OutputBool(value.get_bool() ? "true" : "false");
        break;
    case boost::json::kind::int64: 
        OutputInteger(std::to_string(value.get_int64()));
        break;
    case boost::json::kind::uint64: 
        OutputInteger(std::to_string(value.get_uint64()));
        break;
    case boost::json::kind::double_: 
        FormatDouble(value.get_double());
        break;
    case boost::json::kind::string: 
        OutputString(std::format("\"{}\"", std::string_view{ value.get_string() }));
        break;
    case boost::json::kind::array:
        FormatArray(value.get_array());
        break;
    case boost::json::kind::object:
        FormatObject(value.get_object());
        break;
    default:
        ZAF_NOT_REACHED();
    }
}


void JSONFormatter::FormatDouble(double value) {

    auto string = std::to_string(value);
    auto not_zero_index = string.find_last_not_of('0');
    if (not_zero_index != std::string::npos) {
        if (string[not_zero_index] == '.') {
            string.erase(std::min(not_zero_index + 2, string.size()));
        }
        else {
            string.erase(not_zero_index + 1);
        }
    }

    OutputDouble(string);
}


void JSONFormatter::FormatArray(const boost::json::array& array_value) {

    OutputArrayBegin("[");

    for (auto iterator = array_value.begin(); iterator != array_value.end(); ++iterator) {

        if (iterator != array_value.begin()) {
            OutputComma(",");
        }

        FormatValue(*iterator);
    }

    OutputArrayEnd("]");
}


void JSONFormatter::FormatObject(const boost::json::object& object) {

    OutputObjectBegin("{");

    for (auto iterator = object.begin(); iterator != object.end(); ++iterator) {

        if (iterator != object.begin()) {
            OutputComma(",");
        }

        OutputKey(std::format("\"{}\"", std::string_view{ iterator->key() }));
        OutputColon(":");

        FormatValue(iterator->value());
    }

    OutputObjectEnd("}");
}


JSONBeautifiedFormatter::JSONBeautifiedFormatter() {

    key_style_.SetTextColor(Color::Black());
    string_style_.SetTextColor(Color::FromRGB(0xAA3322));
    number_style_.SetTextColor(Color::Green());
    keyword_style_.SetTextColor(Color::FromRGB(0x0055FF));
}


zaf::textual::StyledText JSONBeautifiedFormatter::Format(const boost::json::value& value) {

    result_ = {};
    result_.SetDefaultFont(Font{ L"Consolas", 16 });
    result_.SetDefaultTextColor(Color::Gray());

    deep_ = 0;

    StartFormatting(value);
    return std::move(result_);
}


void JSONBeautifiedFormatter::OutputNull(std::string_view text) {
    result_.AppendText(zaf::FromUTF8String(text), keyword_style_);
}


void JSONBeautifiedFormatter::OutputBool(std::string_view text) {
    result_.AppendText(zaf::FromUTF8String(text), keyword_style_);
}


void JSONBeautifiedFormatter::OutputInteger(std::string_view text) {
    result_.AppendText(zaf::FromUTF8String(text), number_style_);
}


void JSONBeautifiedFormatter::OutputDouble(std::string_view text) {
    result_.AppendText(zaf::FromUTF8String(text), number_style_);
}


void JSONBeautifiedFormatter::OutputString(std::string_view text) {
    result_.AppendText(zaf::FromUTF8String(text), string_style_);
}


void JSONBeautifiedFormatter::OutputArrayBegin(std::string_view text) {
    EnterLevel(text);
}


void JSONBeautifiedFormatter::OutputArrayEnd(std::string_view text) {
    ExitLevel(text);
}


void JSONBeautifiedFormatter::OutputObjectBegin(std::string_view text) {
    EnterLevel(text);
}


void JSONBeautifiedFormatter::OutputObjectEnd(std::string_view text) {
    ExitLevel(text);
}


void JSONBeautifiedFormatter::OutputKey(std::string_view text) {
    result_.AppendText(zaf::FromUTF8String(text), key_style_);
}


void JSONBeautifiedFormatter::OutputColon(std::string_view text) {
    result_.AppendText(zaf::FromUTF8String(text));
    result_.AppendText(L" ");
}


void JSONBeautifiedFormatter::OutputComma(std::string_view text) {
    result_.AppendText(zaf::FromUTF8String(text));
    result_.AppendText(L"\n");
    OutputIdent();
}


void JSONBeautifiedFormatter::EnterLevel(std::string_view text) {

    result_.AppendText(zaf::FromUTF8String(text));
    result_.AppendText(L"\n");

    ++deep_;
    OutputIdent();
}


void JSONBeautifiedFormatter::ExitLevel(std::string_view text) {

    result_.AppendText(L"\n");

    --deep_;
    OutputIdent();
    
    result_.AppendText(zaf::FromUTF8String(text));
}


void JSONBeautifiedFormatter::OutputIdent() {
    result_.AppendText(std::wstring(4 * deep_, L' '));
}


std::string JSONPrimitiveFormatter::Format(const boost::json::value& value) {

    result_ = {};
    StartFormatting(value);
    return std::move(result_);
}


void JSONPrimitiveFormatter::OutputNull(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputBool(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputInteger(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputDouble(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputString(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputArrayBegin(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputArrayEnd(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputObjectBegin(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputObjectEnd(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputKey(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputColon(std::string_view text) {
    result_.append(text);
}


void JSONPrimitiveFormatter::OutputComma(std::string_view text) {
    result_.append(text);
}

}