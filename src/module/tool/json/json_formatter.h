#pragma once

#include <boost/json.hpp>
#include <zaf/control/textual/styled_text.h>

namespace ra::mod::tool::json {

class JSONFormatter {
public:
    JSONFormatter();

    zaf::textual::StyledText Format(const boost::json::value& value) const;

private:
    void FormatValue(
        const boost::json::value& value, 
        std::size_t deep, 
        zaf::textual::StyledText& styled_text) const;

    void FormatArray(
        const boost::json::array& array_value,
        std::size_t deep,
        zaf::textual::StyledText& styled_text) const;

    void FormatObject(
        const boost::json::object& object, 
        std::size_t deep, 
        zaf::textual::StyledText& styled_text) const;

private:
    zaf::textual::TextStyle key_style_;
    zaf::textual::TextStyle string_style_;
    zaf::textual::TextStyle number_style_;
    zaf::textual::TextStyle keyword_style_;
};

}