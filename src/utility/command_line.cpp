#include "utility/command_line.h"
#include <Windows.h>
#include <zaf/base/container/utility/append.h>
#include <zaf/base/container/utility/range.h>
#include <zaf/base/string/trim.h>

namespace ra::utility {
namespace {

constexpr wchar_t ObjectReplacementChar = L'\ufffc';

std::vector<std::wstring> ExpandObjectText(
    std::wstring_view text, 
    const CommandLine::ObjectTextGetter& object_text_getter,
    int& object_index) {

    std::vector<std::wstring> result;

    std::size_t begin{};
    std::size_t current{};
    for (current = 0; current <= text.length(); ++current) {

        if (current == text.length() || text[current] == ObjectReplacementChar) {

            auto part = std::wstring{ text.substr(begin, current - begin) };
            zaf::Trim(part);

            if (!part.empty()) {
                result.push_back(std::move(part));
            }

            //Encounter object replacement char.
            if (current != text.length()) {

                std::wstring object_text;
                if (object_text_getter) {
                    object_text = object_text_getter(object_index);
                }

                if (!object_text.empty()) {
                    result.push_back(object_text);
                }

                object_index++;
            }

            begin = current + 1;
        }
    }

    return result;
}


std::vector<std::wstring> ParseTextToParts(
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
    std::vector<std::wstring> result;
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

    auto parts = ParseTextToParts(text, object_text_getter);

    for (auto index : zaf::Range(0, parts.size())) {
        if (index == 0) {
            command_ = std::move(parts[index]);
        }
        else {
            arguments_.push_back(std::move(parts[index]));
        }
    }
}


std::vector<std::wstring> CommandLine::AllParts() const {

    std::vector<std::wstring> result;
    if (!command_.empty()) {
        result.push_back(command_);
    }

    zaf::Append(result, arguments_);
    return result;
}

}