#pragma once

#include <string>

namespace calculator {

class ParseContext {
public:
    ParseContext(const std::wstring& command_text) :
        command_text_(command_text),
        current_index_(0) {

    }

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

    bool Forward() {

        if (current_index_ < command_text_.length()) {
            ++current_index_;
            return true;
        }
        else {
            return false;
        }
    }

    bool Backward(std::size_t count = 1) {

        if (current_index_ >= count) {
            current_index_ -= count;
            return true;
        }
        else {
            return false;
        }
    }

private:
    std::wstring command_text_;
    std::size_t current_index_{};
};

}