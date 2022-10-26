#include "module/tool/hex/hex_content_control.h"
#include <zaf/base/container/utility/append.h>
#include <zaf/base/container/utility/range.h>
#include <zaf/base/container/utility/sort.h>
#include <zaf/graphic/canvas.h>
#include <zaf/window/message/mouse_message.h>
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
            renderer.CreateSolidColorBrush(zaf::Color::FromARGB(0x4055BBFF));
    }

    if (selected_background_brush_.IsNull()) {
        selected_background_brush_ = renderer.CreateSolidColorBrush(zaf::Color::FromRGB(0xCCEEBB));
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

    if (!selection_info_) {
        return false;
    }

    return 
        byte_index >= selection_info_->MinByteIndex() && 
        byte_index <= selection_info_->MaxByteIndex();
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

    auto need_repainted_lines = HandleMouseOverBytesOnMouseMove(position);
    zaf::Append(need_repainted_lines, HandleSelectedBytesOnMouseMove(position));

    RepaintLines(need_repainted_lines);
}


std::vector<std::size_t> HexContentControl::HandleMouseOverBytesOnMouseMove(
    const zaf::Point& mouse_position) {

    std::vector<std::size_t> need_repainted_lines;
    if (mouse_over_byte_index_) {
        need_repainted_lines.push_back(mouse_over_byte_index_->Line());
    }

    bool is_in_character_index{};
    mouse_over_byte_index_ = FindByteIndex(mouse_position, false, is_in_character_index);
    if (mouse_over_byte_index_) {
        need_repainted_lines.push_back(mouse_over_byte_index_->Line());
    }

    return need_repainted_lines;
}


std::vector<std::size_t> HexContentControl::HandleSelectedBytesOnMouseMove(
    const zaf::Point& mouse_position) {

    if (!IsCapturingMouse()) {
        return {};
    }

    if (!selection_info_) {
        return {};
    }

    auto revised_position = mouse_position;
    if (!selection_info_->is_in_character_area) {
        revised_position.x = std::max(revised_position.x, ByteHexAreaX());
        revised_position.x = std::min(revised_position.x, ByteHexSecondPartEndX());
    }
    else {
        revised_position.x = std::max(revised_position.x, ByteCharacterAreaX());
        revised_position.x = std::min(revised_position.x, ByteCharacterAreaEndX());
    }

    bool is_in_character_area{};
    auto end_byte_index = FindByteIndex(revised_position, true, is_in_character_area);

    ZAF_EXPECT(end_byte_index);
    ZAF_EXPECT(is_in_character_area == selection_info_->is_in_character_area);

    selection_info_->end_byte_index = *end_byte_index;

    return GetSelectedLines();
}


bool HexContentControl::OnMouseDown(const zaf::Point& position, const zaf::MouseMessage& message) {

    HandleMouseDown(position, message);

    return __super::OnMouseDown(position, message);
}


void HexContentControl::HandleMouseDown(
    const zaf::Point& position, 
    const zaf::MouseMessage& message) {

    auto need_repainted_lines = GetSelectedLines();
    selection_info_.reset();

    if (message.MouseButton() == zaf::MouseButton::Left) {

        bool is_in_character_area{};
        auto selected_byte_index = FindByteIndex(position, false, is_in_character_area);
        if (selected_byte_index) {

            SelectionInfo selection_info;
            selection_info.begin_byte_index = *selected_byte_index;
            selection_info.end_byte_index = *selected_byte_index;
            selection_info.is_in_character_area = is_in_character_area;
            selection_info_ = selection_info;

            zaf::Append(need_repainted_lines, GetSelectedLines());
        }
    }
    
    RepaintLines(need_repainted_lines);

    CaptureMouse();
}


bool HexContentControl::OnMouseUp(const zaf::Point& position, const zaf::MouseMessage& message) {

    HandleMouseUp(position);

    return __super::OnMouseUp(position, message);
}


void HexContentControl::HandleMouseUp(const zaf::Point& position) {

    if (IsCapturingMouse()) {
        ReleaseCapture();
    }
}


std::vector<std::size_t> HexContentControl::GetSelectedLines() const {

    if (!selection_info_) {
        return {};
    }

    std::vector<std::size_t> lines;
    auto min_line = selection_info_->MinByteIndex().Line();
    auto max_line = selection_info_->MaxByteIndex().Line();
    for (auto line : zaf::Range(min_line, max_line + 1)) {
        lines.push_back(line);
    }
    return lines;
}


void HexContentControl::RepaintLines(const std::vector<std::size_t>& lines) {

    zaf::Rect need_repainted_rect;
    for (const auto each_line : lines) {

        zaf::Rect line_rect;
        line_rect.position.x = 0;
        line_rect.position.y = each_line * LineHeight;
        line_rect.size.width = ByteCharacterAreaEndX();
        line_rect.size.height = LineHeight;

        need_repainted_rect.Union(line_rect);
    }

    NeedRepaintRect(need_repainted_rect);
}


std::optional<ByteIndex> HexContentControl::FindByteIndex(
    const zaf::Point& position,
    bool adjust_to_nearest_index,
    bool& is_in_character_area) {

    auto line = FindByteLine(position.y, adjust_to_nearest_index);
    if (!line) {
        return std::nullopt;
    }

    auto index_in_line = FindByteIndexInLine(
        position.x,
        adjust_to_nearest_index,
        is_in_character_area);

    if (!index_in_line) {
        return std::nullopt;
    }

    return ByteIndex{ *line, *index_in_line };
}


std::optional<std::size_t> HexContentControl::FindByteIndexInLine(
    float x, 
    bool adjust_to_nearest_index,
    bool& is_in_character_area) {

    float adjusted_x = x;

    if (adjusted_x < ByteHexAreaX()) {
        if (!adjust_to_nearest_index) {
            return std::nullopt;
        }
        adjusted_x = ByteHexAreaX();
    }

    if (adjusted_x < ByteHexFirstPartEndX()) {
        return static_cast<std::size_t>((x - ByteHexAreaX()) / ByteWidth);
    }

    if (x < ByteHexSecondPartBeginX()) {
        if (!adjust_to_nearest_index) {
            return std::nullopt;
        }
        return BytesPerLine / 2 - 1;
    }

    if (x < ByteHexSecondPartEndX()) {
        return 
            static_cast<std::size_t>((x - ByteHexSecondPartBeginX()) / ByteWidth) +
            BytesPerLine / 2;
    }

    if (x < ByteCharacterAreaX()) {
        if (!adjust_to_nearest_index) {
            return std::nullopt;
        }
        return BytesPerLine - 1;
    }

    if (x < ByteCharacterAreaEndX()) {
        is_in_character_area = true;
        return static_cast<std::size_t>((x - ByteCharacterAreaX()) / CharacterWidth);
    }

    if (!adjust_to_nearest_index) {
        return std::nullopt;
    }

    return BytesPerLine - 1;
}


std::optional<std::size_t> HexContentControl::FindByteLine(float y, bool adjust_to_nearest_index) {

    auto adjusted_y = y;

    if (adjust_to_nearest_index) {

        if (adjusted_y < 0) {
            adjusted_y = 0;
        }
    }

    return static_cast<std::size_t>(adjusted_y / LineHeight);
}

}