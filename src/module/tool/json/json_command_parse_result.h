#pragma once

#include <string>
#include <variant>
#include <zaf/control/textual/styled_text.h>

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
    explicit JSONCommandParseResult(zaf::textual::StyledText styled_text) :
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

    const zaf::textual::StyledText* StyledText() const {
        return std::get_if<zaf::textual::StyledText>(&data_);
    }

private:
    std::variant<zaf::textual::StyledText, ErrorInfo> data_;
};

}