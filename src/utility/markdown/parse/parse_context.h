#pragma once

#include <optional>
#include <string_view>
#include <zaf/base/non_copyable.h>

namespace ra::utility::markdown::parse {

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
    explicit ParseContext(std::wstring_view input);

    std::size_t CurrentIndex() const {
        return current_index_;
    }

    wchar_t CurrentChar() const;
    std::optional<wchar_t> PreviousChar() const;

    bool IsAtLineStart() const;
    bool IsAtLineEnd() const;

    bool IsEnd() const {
        return current_index_ == input_.length();
    }

    [[nodiscard]]
    Transaction BeginTransaction() {
        return Transaction{ *this };
    }

    bool Forward();
    void SkipSpaces();
    bool ParseText(std::wstring_view text);

private:
    void Rollback(std::size_t index);

private:
    std::wstring_view input_;
    std::size_t current_index_{};
    const wchar_t* current_pointer_{};
};

}