#include "module/tool/hex/paint_common.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/to_string.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/text/text_format_properties.h>
#include <zaf/graphic/text/text_layout_properties.h>

namespace ra::mod::tool::hex {
namespace {

zaf::TextFormat CreateTextFormat() {

    zaf::TextFormatProperties text_format_properties;
    text_format_properties.font_family_name = L"Consolas";
    text_format_properties.font_size = 14;
    return zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);
}

}


zaf::TextLayout CreateCommonTextLayout(const std::wstring& text, float layout_width) {

    zaf::TextLayoutProperties text_layout_properties;
    text_layout_properties.text = text;
    text_layout_properties.text_format = CreateTextFormat();
    text_layout_properties.width = layout_width;
    text_layout_properties.height = LineHeight;
    auto text_layout = zaf::GraphicFactory::Instance().CreateTextLayout(
        text_layout_properties);

    text_layout.SetTextAlignment(zaf::TextAlignment::Center);
    text_layout.SetParagraphAlignment(zaf::ParagraphAlignment::Center);
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