#pragma once

#include <optional>
#include <string_view>
#include <zaf/base/non_copyable.h>

namespace ra::help::markdown::parse {

class ParseContext : zaf::NonCopyableNonMovable {
public:
    class Transaction : zaf::NonCopyable {
    public:
        explicit Transaction(ParseContext& context) : 
            context_(&context), 
            index_(context.current_index_) {
        
        }

        ~Transaction() {
            TryToRollback();
        }

        Transaction(Transaction&& other) :
            context_(other.context_), 
            index_(other.index_) {

            other.context_ = nullptr;
        }

        Transaction& operator=(Transaction&& other) {
            TryToRollback();
            context_ = other.context_;
            index_ = other.index_;
            other.context_ = nullptr;
            return *this;
        }

        void Commit() {
            context_ = nullptr;
        }

    private:
        void TryToRollback() {
            if (context_) {
                context_->Rollback(index_);
            }
        }

    private:
        ParseContext* context_{};
        std::size_t index_{};
    };

public:
    explicit ParseContext(std::wstring_view input) : input_(input) {

    }

    std::size_t CurrentIndex() const {
        return current_index_;
    }

    wchar_t CurrentChar() const {
        return input_[current_index_];
    }

    std::optional<wchar_t> PreviousChar() const {
        if (current_index_ == 0) {
            return std::nullopt;
        }
        return input_[current_index_ - 1];
    }

    bool IsAtLineStart() const {
        if (current_index_ == 0) {
            return true;
        }
        return input_[current_index_ - 1] == L'\n';
    }

    bool IsAtLineEnd() const {
        auto current_char = CurrentChar();
        return current_char == L'\n' || current_char == L'\0';
    }

    [[nodiscard]]
    Transaction BeginTransaction() {
        return Transaction{ *this };
    }

    bool Forward() {
        if (current_index_ == input_.length()) {
            return false;
        }
        current_index_++;
        return true;
    }

    void SkipSpaces() {
        do {
            if (CurrentChar() != L' ') {
                break;
            }
        }
        while (Forward());
    }

private:
    void Rollback(std::size_t index) {
        current_index_ = index;
    }

private:
    std::wstring input_;
    std::size_t current_index_{};
};

}