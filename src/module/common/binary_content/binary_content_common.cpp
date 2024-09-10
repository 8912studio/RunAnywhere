#include "module/common/binary_content/binary_content_common.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/to_string.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/dwrite/text_format_properties.h>

namespace ra::mod {
namespace {

zaf::dwrite::TextFormat CreateTextFormat() {

    zaf::dwrite::TextFormatProperties text_format_properties;
    text_format_properties.font_family_name = L"Consolas";
    text_format_properties.font_size = 14;
    return zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);
}

}


zaf::dwrite::TextLayout CreateCommonTextLayout(const std::wstring& text, float layout_width) {

    auto text_layout = zaf::GraphicFactory::Instance().CreateTextLayout(
        text,
        CreateTextFormat(),
        zaf::Size{ layout_width, LineHeight });

    text_layout.SetTextAlignment(zaf::dwrite::TextAlignment::Center);
    text_layout.SetParagraphAlignment(zaf::dwrite::ParagraphAlignment::Center);
    return text_layout;
}


std::wstring ToHexString(std::size_t value, std::size_t min_length) {

    auto result = zaf::ToWideString(value, zaf::ToStringOptions().Base(16));
    zaf::Uppercase(result);

    if (result.length() < min_length) {
        result = std::wstring(min_length - result.length(), L'0') + result;
    }

    return result;
}

}