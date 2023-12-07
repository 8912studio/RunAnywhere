#pragma once

#include <boost/json.hpp>
#include "utility/markdown/render/styled_text.h"

namespace ra::mod::tool::json {

class JSONFormatter {
public:
    JSONFormatter();

    utility::markdown::render::StyledText Format(const boost::json::value& value) const;

private:
    void FormatValue(
        const boost::json::value& value, 
        std::size_t deep, 
        utility::markdown::render::StyledText& styled_text) const;

    void FormatArray(
        const boost::json::array& array_value,
        std::size_t deep,
        utility::markdown::render::StyledText& styled_text) const;

    void FormatObject(
        const boost::json::object& object, 
        std::size_t deep, 
        utility::markdown::render::StyledText& styled_text) const;

private:
    utility::markdown::render::TextStyle default_style_;
    utility::markdown::render::TextStyle key_style_;
    utility::markdown::render::TextStyle string_style_;
    utility::markdown::render::TextStyle number_style_;
    utility::markdown::render::TextStyle keyword_style_;
};

}