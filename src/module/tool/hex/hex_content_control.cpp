#include "module/tool/hex/hex_content_control.h"
#include <zaf/base/container/utility/range.h>
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>
#include "module/tool/hex/paint_common.h"

namespace ra::module::tool::hex {
namespace {

zaf::Rect GetByteHexRect(const ByteIndex& byte_index) {

    zaf::Rect result;
    result.position.x = 
        ByteHexAreaX() +
        byte_index.IndexInLine() * ByteWidth +
        (byte_index.IndexInLine() >= BytesPerLine / 2 ? ByteGapWidth : 0);
    result.position.y = byte_index.Line() * LineHeight;
    result.size.width = ByteWidth;
    result.size.height = LineHeight;
    return result;
}


zaf::Rect GetByteCharacterRect(const ByteIndex& byte_index) {

    zaf::Rect result;
    result.position.x = ByteCharacterAreaX() + byte_index.IndexInLine() * CharacterWidth;
    result.position.y = byte_index.Line() * LineHeight;
    result.size.width = CharacterWidth;
    result.size.height = LineHeight;
    return result;
}

}

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
        for (auto byte_index_in_line: zaf::Range<std::size_t>(0, BytesPerLine)) {
            
            ByteIndex byte_index{ line_index, byte_index_in_line };
            if (byte_index.IndexInContent() >= content_.size()) {
                break;
            }

            if (byte_index_in_line == 0) {
                PaintLineHeader(canvas, line_index);
            }

            PaintByteHex(canvas, byte_index);
            PaintByteCharacter(canvas, byte_index);
        }
    }
}


void HexContentControl::PrepareGraphicResources(zaf::Renderer& renderer) {

    if (mouse_over_background_brush_.IsNull()) {
        mouse_over_background_brush_ = 
            renderer.CreateSolidColorBrush(zaf::Color::FromRGB(0xDDEEFF));
    }

    if (selected_background_brush_.IsNull()) {
        selected_background_brush_ = renderer.CreateSolidColorBrush(zaf::Color::Green());
    }

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

    mouse_over_background_brush_.Reset();
    selected_background_brush_.Reset();
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


void HexContentControl::PaintByteHex(zaf::Canvas& canvas, const ByteIndex& byte_index) {

    auto paint_rect = GetByteHexRect(byte_index);

    if (IsByteSelected(byte_index)) {
        canvas.SetBrush(selected_background_brush_);
        canvas.DrawRectangle(paint_rect);
    }

    if (mouse_over_byte_index_ && *mouse_over_byte_index_ == byte_index) {
        canvas.SetBrush(mouse_over_background_brush_);
        canvas.DrawRectangle(paint_rect);
    }

    auto text_layout = GetByteHexTextLayout(content_[byte_index.IndexInContent()]);

    canvas.SetBrush(default_text_brush_);
    canvas.DrawTextLayout(text_layout, paint_rect.position);
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


void HexContentControl::PaintByteCharacter(zaf::Canvas& canvas, const ByteIndex& byte_index) {

    auto paint_rect = GetByteCharacterRect(byte_index);

    if (IsByteSelected(byte_index)) {
        canvas.SetBrush(selected_background_brush_);
        canvas.DrawRectangle(paint_rect);
    }

    if (mouse_over_byte_index_ && *mouse_over_byte_index_ == byte_index) {
        canvas.SetBrush(mouse_over_background_brush_);
        canvas.DrawRectangle(paint_rect);
    }

    auto byte = content_[byte_index.IndexInContent()];

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

    canvas.SetBrush(text_brush);
    canvas.DrawTextLayout(text_layout, paint_rect.position);
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


bool HexContentControl::IsByteSelected(const ByteIndex& byte_index) const {

    if (!selected_byte_index_range_) {
        return false;
    }

    return 
        selected_byte_index_range_->First() <= byte_index && 
        byte_index <= selected_byte_index_range_->Last();
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


bool HexContentControl::OnMouseMove(const zaf::Point& position, const zaf::MouseMessage& message) {

    HandleMouseMove(position);

    return __super::OnMouseMove(position, message);
}


void HexContentControl::OnMouseLeave(const std::shared_ptr<zaf::Control>& leaved_control) {

    __super::OnMouseLeave(leaved_control);

    if (!mouse_over_byte_index_) {
        return;
    }

    auto old_byte_index = *mouse_over_byte_index_;
    mouse_over_byte_index_.reset();

    auto update_guard = BeginUpdate();
    NeedRepaintRect(GetByteHexRect(old_byte_index));
    NeedRepaintRect(GetByteCharacterRect(old_byte_index));
}


void HexContentControl::HandleMouseMove(const zaf::Point& position) {

    std::vector<zaf::Rect> need_repainted_rects;

    if (mouse_over_byte_index_) {
        need_repainted_rects.push_back(GetByteHexRect(*mouse_over_byte_index_));
        need_repainted_rects.push_back(GetByteCharacterRect(*mouse_over_byte_index_));
    }

    auto new_byte_index = FindByteIndex(position);
    if (new_byte_index) {
        
        need_repainted_rects.push_back(GetByteHexRect(*new_byte_index));
        need_repainted_rects.push_back(GetByteCharacterRect(*new_byte_index));
    }

    mouse_over_byte_index_ = new_byte_index;

    auto update_guard = BeginUpdate();
    for (const auto& each_rect : need_repainted_rects) {
        NeedRepaintRect(each_rect);
    }
}


bool HexContentControl::OnMouseDown(const zaf::Point& position, const zaf::MouseMessage& message) {

    HandleMouseDown(position);

    return __super::OnMouseDown(position, message);
}


void HexContentControl::HandleMouseDown(const zaf::Point& position) {

    auto selected_byte_index = FindByteIndex(position);
    if (!selected_byte_index) {
        selected_byte_index_range_.reset();
        return;
    }

    selected_byte_index_range_ = ByteIndexRange{ *selected_byte_index , *selected_byte_index };
}


bool HexContentControl::OnMouseUp(const zaf::Point& position, const zaf::MouseMessage& message) {

    HandleMouseUp(position);

    return __super::OnMouseUp(position, message);
}


void HexContentControl::HandleMouseUp(const zaf::Point& position) {

}


std::optional<ByteIndex> HexContentControl::FindByteIndex(const zaf::Point& position) const {

    auto index_in_line = FindByteIndexInLine(position.x);
    if (!index_in_line) {
        return std::nullopt;
    }

    auto line = static_cast<std::size_t>(position.y / LineHeight);

    return ByteIndex{ line, *index_in_line };
}


std::optional<std::size_t> HexContentControl::FindByteIndexInLine(float x) const {

    if (x < ByteHexAreaX()) {
        return std::nullopt;
    }

    constexpr auto hex_first_part_end = ByteHexAreaX() + (BytesPerLine / 2) * ByteWidth;
    if (x < hex_first_part_end) {
        return static_cast<std::size_t>((x - ByteHexAreaX()) / ByteWidth);
    }

    constexpr auto hex_second_part_begin = hex_first_part_end + ByteGapWidth;
    if (x < hex_second_part_begin) {
        return std::nullopt;
    }

    constexpr auto byte_hex_second_part_end = 
        hex_second_part_begin + 
        (BytesPerLine / 2) * ByteWidth;

    if (x < byte_hex_second_part_end) {
        return 
            static_cast<std::size_t>((x - hex_second_part_begin) / ByteWidth) +
            BytesPerLine / 2;
    }

    if (x < ByteCharacterAreaX()) {
        return std::nullopt;
    }

    constexpr auto character_part_end = ByteCharacterAreaX() + BytesPerLine * CharacterWidth;
    if (x < character_part_end) {
        return static_cast<std::size_t>((x - ByteCharacterAreaX()) / CharacterWidth);
    }

    return std::nullopt;
}

}