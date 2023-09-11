#pragma once

#include <optional>
#include <string>
#include <string_view>
#include "module/calculator/parse/parse_error_reason.h"
#include "module/calculator/parse/parse_reader.h"

namespace ra::mod::calculator {

class ParseContext {
public:
    ParseContext(std::wstring_view command_text);

    ParseContext(const ParseContext&) = delete;
    ParseContext& operator=(const ParseContext&) = delete;

    std::size_t GetLength() const {
        return command_text_.length();
    }

    std::size_t GetCurrentIndex() const {
        return current_index_;
    }

    wchar_t GetCurrentChar() const {
        return command_text_[current_index_];
    }

    std::optional<wchar_t> GetCharAtOffset(int offset) const;

    void Forward(std::size_t length);

    void SkipWhiteSpaces();

    std::size_t GetLastParsedLength() const {
        return last_parsed_length_;
    }

    void PushParenthesis() {
        ++parenthesis_deep;
    }

    void PopParenthesis() {
        --parenthesis_deep;
    }

    int GetParenthesisDeep() const {
        return parenthesis_deep;
    }

    ParseReader BeginRead();

    ParseErrorReason GetParseErrorReason() const {
        return error_reason_;
    }

    void SetParseErrorReason(ParseErrorReason reason) {
        error_reason_ = reason;
    }

private:
    std::wstring command_text_;
    std::size_t current_index_{};

    std::size_t last_parsed_length_{};
    int parenthesis_deep{};
    ParseErrorReason error_reason_{ ParseErrorReason::Unspecified };
};

}