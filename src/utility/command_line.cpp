#include "utility/command_line.h"
#include <Windows.h>
#include <zaf/base/container/utility/append.h>
#include <zaf/base/range.h>
#include <zaf/base/string/trim.h>

namespace ra::utility {
namespace {

constexpr wchar_t ObjectReplacementChar = L'\ufffc';

std::vector<CommandLinePiece> ExpandObjectText(
    std::wstring_view text, 
    const CommandLine::ObjectTextGetter& object_text_getter,
    int& object_index) {

    std::vector<CommandLinePiece> result;

    std::size_t begin{};
    std::size_t current{};
    for (current = 0; current <= text.length(); ++current) {

        if (current == text.length() || text[current] == ObjectReplacementChar) {

            //Handle normal text.
            auto piece_content = std::wstring{ text.substr(begin, current - begin) };
            zaf::Trim(piece_content);

            if (!piece_content.empty()) {
                result.push_back(CommandLinePiece{ std::move(piece_content) });
            }

            //Encounter object replacement char.
            if (current != text.length()) {

                CommandLinePiece piece;
                if (object_text_getter) {
                    piece = object_text_getter(object_index);
                    ZAF_EXPECT(piece.Type() != CommandLinePieceType::NormalText);
                }

                if (!piece.Content().empty()) {
                    result.push_back(std::move(piece));
                }

                object_index++;
            }

            begin = current + 1;
        }
    }

    return result;
}


std::vector<CommandLinePiece> ParseTextToPieces(
    std::wstring_view text,
    const CommandLine::ObjectTextGetter& object_text_getter) {

    //It is strange that CommandLineToArgvW would returns the exe file path if text is an empty 
    //string.
    if (text.empty()) {
        return {};
    }

    int count{};
    auto parts = CommandLineToArgvW(text.data(), &count);
    if (!parts) {
        return {};
    }

    int object_index{};
    std::vector<CommandLinePiece> result;
    for (auto index : zaf::Range(0, count)) {

        auto expanded_parts = ExpandObjectText(parts[index], object_text_getter, object_index);
        zaf::Append(result, expanded_parts);
    }

    LocalFree(parts);
    return result;
}

}

CommandLine::CommandLine(
    std::wstring_view text,
    const ObjectTextGetter& object_text_getter)
    :
    raw_text_(text) {

    pieces_ = ParseTextToPieces(text, object_text_getter);
}


std::wstring CommandLine::Command() const {

    if (FirstPieceIsCommand()) {
        return pieces_.front().Content();
    }

    return {};
}


bool CommandLine::FirstPieceIsCommand() const {

    if (!pieces_.empty()) {
        return pieces_.front().Type() == CommandLinePieceType::NormalText;
    }
    return false;
}


std::vector<CommandLinePiece> CommandLine::Arguments() const {

    if (!FirstPieceIsCommand()) {
        return {};
    }

    std::vector<CommandLinePiece> result;
    for (std::size_t index = 1; index < pieces_.size(); ++index) {
        result.push_back(pieces_[index]);
    }
    return result;
}


std::vector<std::wstring> CommandLine::AllPieceContents() const {

    std::vector<std::wstring> result;
    for (const auto& each_piece : pieces_) {
        result.push_back(each_piece.Content());
    }
    return result;
}

}