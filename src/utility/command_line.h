#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <zaf/base/non_copyable.h>

namespace ra::utility {

constexpr wchar_t ObjectReplacementChar = L'\ufffc';


enum class CommandLinePieceType {
    NormalText,
    TextBlock,
    ActivePath,
};


class CommandLinePiece {
public:
    CommandLinePiece() = default;

    explicit CommandLinePiece(std::wstring content) : content_(std::move(content)) { }

    CommandLinePiece(CommandLinePieceType type, std::wstring content) :
        type_(type), 
        content_(std::move(content)) {

    }

    CommandLinePieceType Type() const {
        return type_;
    }

    const std::wstring& Content() const {
        return content_;
    }

    friend auto operator<=>(const CommandLinePiece&, const CommandLinePiece&) = default;

private:
    CommandLinePieceType type_{ CommandLinePieceType::NormalText };
    std::wstring content_;
};


class CommandLine : zaf::NonCopyable {
public:
    using ObjectTextGetter = std::function<CommandLinePiece(int object_index)>;

public:
    explicit CommandLine(
        std::wstring_view text, 
        const ObjectTextGetter& object_text_getter = nullptr);

    const std::wstring& RawText() const {
        return raw_text_;
    }

    std::wstring Command() const;
    std::vector<CommandLinePiece> Arguments() const;
    const std::vector<CommandLinePiece>& AllPieces() const {
        return pieces_;
    }

    std::vector<std::wstring> AllPieceContents() const;

private:
    bool FirstPieceIsCommand() const;

private:
    std::wstring raw_text_;
    std::vector<CommandLinePiece> pieces_;
};

}