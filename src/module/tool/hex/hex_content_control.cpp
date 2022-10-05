#include "module/tool/hex/hex_content_control.h"
#include <zaf/base/container/utility/range.h>
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>
#include "module/tool/hex/paint_common.h"

namespace ra::module::tool::hex {

ZAF_DEFINE_TYPE(HexContentControl)
ZAF_DEFINE_TYPE_END;


void HexContentControl::SetContent(const std::vector<std::byte>& content) {

    content_ = content;
    NeedRepaint();
    RaiseContentChangedEvent();
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

    if (default_text_brush_.IsNull()) {
        default_text_brush_ = renderer.CreateSolidColorBrush(zaf::Color::Black());
    }

    if (blank_character_brush_.IsNull()) {
        blank_character_brush_ = renderer.CreateSolidColorBrush(zaf::Color::FromRGB(0x00AA00));
    }

    if (unknown_character_brush_.IsNull()) {
        unknown_character_brush_ = renderer.CreateSolidColorBrush(zaf::Color::FromRGB(0xAAAAAA));
    }
}


void HexContentControl::ReleaseRendererResources() {

    default_text_brush_.Reset();
    blank_character_brush_.Reset();
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
    if (std::isspace(static_cast<int>(byte))) {
        character = L'.';
        text_brush = blank_character_brush_;
    }
    else if (std::isprint(static_cast<int>(byte))) {
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