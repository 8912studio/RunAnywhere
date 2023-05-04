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

        auto ch = text[current];
        if (ch == ObjectReplacementChar || ch == '\0') {

            auto part_view = text.substr(begin, current - begin);
            std::wstring part{ part_view.data(), part_view.length() };
            zaf::Trim(part);

            if (!part.empty()) {
                result.push_back(std::move(part));
            }

            if (ch == ObjectReplacementChar) {

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
    text_(text) {

    int count{};
    auto parts = CommandLineToArgvW(text.data(), &count);
    if (!parts) {
        return;
    }

    for (auto index : zaf::Range(0, count)) {

        all_parts_.push_back(parts[index]);

        if (index == 0) {
            command_ = parts[index];
        }
        else {
            arguments_.push_back(parts[index]);
        }
    }

    LocalFree(parts);
}

}