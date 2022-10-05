#include "module/tool/hex/hex_content_control.h"
#include <zaf/base/container/utility/range.h>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/to_string.h>
#include <zaf/graphic/canvas.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/text/text_format_properties.h>
#include <zaf/graphic/text/text_layout_properties.h>
#include <zaf/object/type_definition.h>

namespace ra::module::tool::hex {
namespace {

constexpr std::size_t BytesPerLine = 16;
constexpr float LineHeaderWidth = 30;
constexpr float LineHeaderMargin = 6;
constexpr float ByteWidth = 22;
constexpr float ByteGapWidth = 8;
constexpr float CharacterAreaGapWidth = 22;
constexpr float CharacterWidth = 10;


constexpr float ByteHexAreaX() {
    return LineHeaderWidth + LineHeaderMargin;
}


constexpr float ByteChatacterAreaX() {
    return ByteHexAreaX() + BytesPerLine * ByteWidth + ByteGapWidth + CharacterAreaGapWidth;
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


ZAF_DEFINE_TYPE(HexContentControl)
ZAF_DEFINE_TYPE_END;


std::wstring HexContentControl::FontName() {
    return L"Consolas";
}


zaf::Color HexContentControl::HeaderBackgroundColor() {
    return zaf::Color::FromRGB(0xEEEEEE);
}


zaf::Color HexContentControl::HeaderTextColor() {
    return zaf::Color::FromRGB(0x999999);
}


void HexContentControl::SetContent(const std::vector<std::byte>& content) {

    content_ = content;
    NeedRepaint();
}


void HexContentControl::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);

    if (content_.empty()) {
        return;
    }

    PrepareGraphicResources(canvas.GetRenderer());

    auto begin_line = static_cast<std::size_t>(std::floor(dirty_rect.position.y / LineHeight));
    auto end_line = static_cast<std::size_t>(
        std::floor((dirty_rect.position.y + dirty_rect.size.height) / LineHeight) + 1);

    for (auto line_index : zaf::Range(begin_line, end_line)) {
        for (auto byte_index_in_line: zaf::Range(0, BytesPerLine)) {
            
            auto byte_index_in_content = line_index * BytesPerLine + byte_index_in_line;
            if (byte_index_in_content >= content_.size()) {
                break;
            }

            if (byte_index_in_line == 0) {
                PaintLineHeader(canvas, line_index);
            }

            auto byte = content_[byte_index_in_content];
            PaintByteHex(canvas, byte, line_index, byte_index_in_line);
            PaintByteCharacter(canvas, byte, line_index, byte_index_in_line);
        }
    }
}


void HexContentControl::PrepareGraphicResources(zaf::Renderer& renderer) {

    if (text_format_.IsNull()) {

        zaf::TextFormatProperties text_format_properties;
        text_format_properties.font_family_name = FontName();
        text_format_properties.font_size = 14;
        text_format_ = zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);
    }

    if (default_text_brush_.IsNull()) {
        default_text_brush_ = renderer.CreateSolidColorBrush(zaf::Color::Black());
    }

    if (unknown_character_brush_.IsNull()) {
        unknown_character_brush_ = renderer.CreateSolidColorBrush(zaf::Color::Gray());
    }
}


void HexContentControl::ReleaseRendererResources() {

    default_text_brush_.Reset();
    unknown_character_brush_.Reset();
}


void HexContentControl::PaintLineHeader(
    zaf::Canvas& canvas, 
    std::size_t line_index) {

    zaf::Rect paint_rect;
    paint_rect.position.x = 0;
    paint_rect.position.y = line_index * LineHeight;
    paint_rect.size.width = LineHeaderWidth;
    paint_rect.size.height = LineHeight;

    canvas.SetBrushWithColor(HeaderBackgroundColor());
    canvas.DrawRectangle(paint_rect);

    auto paint_text = ToHexString(line_index * BytesPerLine, 3);
    auto text_layout = CreateCommonTextLayout(paint_text, paint_rect.size.width);

    canvas.SetBrushWithColor(HeaderTextColor());
    canvas.DrawTextLayout(text_layout, paint_rect.position);
}


void HexContentControl::PaintByteHex(
    zaf::Canvas& canvas,
    std::byte byte,
    std::size_t line_index,
    std::size_t byte_index_in_line) {

    auto text_layout = GetByteHexTextLayout(byte);

    zaf::Point paint_position;
    paint_position.x = 
        ByteHexAreaX() + 
        byte_index_in_line * ByteWidth +
        (byte_index_in_line >= BytesPerLine / 2 ? ByteGapWidth : 0);

    paint_position.y = line_index * LineHeight;

    canvas.SetBrush(default_text_brush_);
    canvas.DrawTextLayout(text_layout, paint_position);
}


zaf::TextLayout HexContentControl::GetByteHexTextLayout(std::byte byte) {

    auto iterator = byte_hex_text_layouts_.lower_bound(byte);
    if (iterator == byte_hex_text_layouts_.end() ||
        iterator->first != byte) {

        auto byte_text = ToHexString(static_cast<std::uint8_t>(byte), 2);

        auto text_layout = CreateCommonTextLayout(byte_text, ByteWidth);
        iterator = byte_hex_text_layouts_.insert(iterator, std::make_pair(byte, text_layout));
    }

    return iterator->second;
}


void HexContentControl::PaintByteCharacter(
    zaf::Canvas& canvas,
    std::byte byte,
    std::size_t line_index,
    std::size_t byte_index_in_line) {

    wchar_t character{};
    zaf::Brush text_brush;
    if (std::isprint(static_cast<int>(byte))) {
        character = static_cast<wchar_t>(byte);
        text_brush = default_text_brush_;
    }
    else {
        character = L'.';
        text_brush = unknown_character_brush_;
    }

    auto text_layout = GetByteCharacterTextLayout(character);

    zaf::Point draw_position;
    draw_position.x = ByteChatacterAreaX() + byte_index_in_line * CharacterWidth;
    draw_position.y = line_index * LineHeight;

    canvas.SetBrush(text_brush);
    canvas.DrawTextLayout(text_layout, draw_position);
}


zaf::TextLayout HexContentControl::GetByteCharacterTextLayout(wchar_t character) {

    auto iterator = byte_character_text_layouts_.find(character);
    if (iterator == byte_character_text_layouts_.end() ||
        iterator->first != character) {

        auto text_layout = CreateCommonTextLayout(std::wstring(1, character), CharacterWidth);

        iterator = byte_character_text_layouts_.insert(
            iterator,
            std::make_pair(character, text_layout));
    }

    return iterator->second;
}


zaf::TextLayout HexContentControl::CreateCommonTextLayout(
    const std::wstring& text, 
    float width) const {

    zaf::TextLayoutProperties text_layout_properties;
    text_layout_properties.text = text;
    text_layout_properties.text_format = text_format_;
    text_layout_properties.width = width;
    text_layout_properties.height = LineHeight;
    auto text_layout = zaf::GraphicFactory::Instance().CreateTextLayout(
        text_layout_properties);

    text_layout.SetTextAlignment(zaf::TextAlignment::Center);
    text_layout.SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    return text_layout;
}


zaf::Size HexContentControl::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    std::size_t line_count = content_.size() / BytesPerLine;;
    if (content_.size() % BytesPerLine > 0) {
        line_count++;
    }

    zaf::Size result;
    result.width = bound_size.width;
    result.height = line_count * LineHeight;
    return result;
}

}