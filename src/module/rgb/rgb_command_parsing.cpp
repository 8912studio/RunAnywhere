#include "module/rgb/rgb_command_parsing.h"
#include <zaf/base/container/utility/range.h>
#include <zaf/base/string/split.h>
#include <zaf/base/string/to_numeric.h>
#include <zaf/base/string/trim.h>
#include "utility/command_line_arguments.h"

namespace ra::module::rgb {
namespace {

zaf::Color ParseColor(const std::wstring& color_argument) {

    auto trimmed = zaf::ToTrimmed(color_argument, [](wchar_t ch) {
        return ch == L',';
    });

    auto splitted_parts = zaf::Split(trimmed, L',');

    std::uint8_t rgb_value[3]{};

    for (auto index : zaf::Range(0u, std::min(std::size(rgb_value), splitted_parts.size()))) {

        if (!zaf::TryToNumeric(splitted_parts[index], rgb_value[index])) {
            return {};
        }
    }

    return zaf::Color::FromCOLORREF(RGB(rgb_value[0], rgb_value[1], rgb_value[2]));
}

}

std::optional<RGBCommandParseResult> ParseRGBCommand(const std::wstring& command_text) {

    auto arguments = utility::CommandLineArguments::Parse(command_text);
    if (arguments.GetCount() <= 0) {
        return std::nullopt;
    }

    if (arguments[0] != L"rgb") {
        return std::nullopt;
    }

    RGBCommandParseResult result;
    
    if (arguments.GetCount() > 1) {
        result.color = ParseColor(arguments[1]);
    }

    return result;
}

}