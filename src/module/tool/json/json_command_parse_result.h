#pragma once

#include <string>
#include <variant>
#include "utility/markdown/render/styled_text.h"

namespace ra::mod::tool::json {

class JSONCommandParseResult {
public:
    class ErrorInfo {
    public:
        std::error_code code;
        std::size_t last_parsed_index{};
        std::wstring original_text;
    };

public:
    explicit JSONCommandParseResult(utility::markdown::render::StyledText styled_text) :
        data_(std::move(styled_text)) {

    }

    explicit JSONCommandParseResult(
        std::error_code error_code, 
        std::size_t last_parsed_index,
        std::wstring original_text)
        : 
        data_(ErrorInfo{ error_code, last_parsed_index, original_text }) {

    }

    const ErrorInfo* Error() const {
        return std::get_if<ErrorInfo>(&data_);
    }

    const utility::markdown::render::StyledText* StyledText() const {
        return std::get_if<utility::markdown::render::StyledText>(&data_);
    }

private:
    std::variant<utility::markdown::render::StyledText, ErrorInfo> data_;
};

}