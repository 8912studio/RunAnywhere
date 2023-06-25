#include "module/common/binary_content/binary_column_body.h"
#include <zaf/base/container/utility/append.h>
#include <zaf/base/container/utility/range.h>
#include <zaf/base/container/utility/sort.h>
#include <zaf/graphic/canvas.h>
#include <zaf/window/message/mouse_message.h>
#include <zaf/object/type_definition.h>
#include "module/common/binary_content/binary_content_common.h"

namespace ra::mod {
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

ZAF_DEFINE_TYPE(BinaryColumnBody)
ZAF_DEFINE_TYPE_END;


void BinaryColumnBody::SetBinary(std::vector<std::byte> binary) {

    content_ = std::move(binary);
    NeedRepaint();
    RaiseContentChangedEvent();
}


void BinaryColumnBody::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);

    if (content_.empty()) {
        return;
    }

    PrepareGraphicResources(canvas.Renderer());

    auto begin_line = static_cast<std::size_t>(std::floor(dirty_rect.position.y / LineHeight));
    auto end_line = static_cast<std::size_t>(
        std::floor((dirty_rect.position.y + dirty_rect.size.height) / LineHeight) + 1);

    for (auto line_index : zaf::Range(begin_line, end_line)) {
        for (auto byte_index_in_line : zaf::Range<std::size_t>(0, BytesPerLine)) {

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


void BinaryColumnBody::PrepareGraphicResources(zaf::Renderer& renderer) {

    if (!mouse_over_background_brush_) {
        mouse_over_background_brush_ =
            renderer.CreateSolidColorBrush(zaf::Color::FromARGB(0x4055BBFF));
    }

    if (!selected_background_brush_) {
        selected_background_brush_ = renderer.CreateSolidColorBrush(zaf::Color::FromRGB(0xCCEEBB));
    }

    if (!default_text_brush_) {
        default_text_brush_ = renderer.CreateSolidColorBrush(zaf::Color::Black());
    }

    if (!blank_character_brush_) {
        blank_character_brush_ = renderer.CreateSolidColorBrush(zaf::Color::FromRGB(0x00AA00));
    }

    if (!unknown_character_brush_) {
        unknown_character_brush_ = renderer.CreateSolidColorBrush(zaf::Color::FromRGB(0xAAAAAA));
    }
}


void BinaryColumnBody::ReleaseRendererResources() {

    mouse_over_background_brush_ = {};
    selected_background_brush_ = {};
    default_text_brush_ = {};
    blank_character_brush_ = {};
    unknown_character_brush_ = {};
}


void BinaryColumnBody::PaintLineHeader(
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


void BinaryColumnBody::PaintByteHex(zaf::Canvas& canvas, const ByteIndex& byte_index) {

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


zaf::TextLayout BinaryColumnBody::GetByteHexTextLayout(std::byte byte) {

    auto iterator = byte_hex_text_layouts_.lower_bound(byte);
    if (iterator == byte_hex_text_layouts_.end() ||
        iterator->first != byte) {

        auto byte_text = ToHexString(static_cast<std::uint8_t>(byte), 2);

        auto text_layout = CreateCommonTextLayout(byte_text, ByteWidth);
        iterator = byte_hex_text_layouts_.insert(iterator, std::make_pair(byte, text_layout));
    }

    return iterator->second;
}


void BinaryColumnBody::PaintByteCharacter(zaf::Canvas& canvas, const ByteIndex& byte_index) {

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


zaf::TextLayout BinaryColumnBody::GetByteCharacterTextLayout(wchar_t character) {

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


bool BinaryColumnBody::IsByteSelected(const ByteIndex& byte_index) const {

    if (!selection_info_) {
        return false;
    }

    return
        byte_index >= selection_info_->MinByteIndex() &&
        byte_index <= selection_info_->MaxByteIndex();
}


zaf::Size BinaryColumnBody::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    std::size_t line_count = content_.size() / BytesPerLine;
    if (content_.size() % BytesPerLine > 0) {
        line_count++;
    }

    zaf::Size result;
    result.width = bound_size.width;
    result.height = line_count * LineHeight;
    return result;
}


void BinaryColumnBody::OnMouseMove(const zaf::MouseMoveInfo& event_info) {

    __super::OnMouseMove(event_info);

    HandleMouseMove(event_info.PositionAtSender());
    event_info.MarkAsHandled();
}


void BinaryColumnBody::OnMouseLeave(const zaf::MouseLeaveInfo& event_info) {

    __super::OnMouseLeave(event_info);

    if (!mouse_over_byte_index_) {
        return;
    }

    auto old_byte_index = *mouse_over_byte_index_;
    mouse_over_byte_index_.reset();

    auto update_guard = BeginUpdate();
    NeedRepaintRect(GetByteHexRect(old_byte_index));
    NeedRepaintRect(GetByteCharacterRect(old_byte_index));

    event_info.MarkAsHandled();
}


void BinaryColumnBody::HandleMouseMove(const zaf::Point& position) {

    auto need_repainted_lines = HandleMouseOverBytesOnMouseMove(position);
    zaf::Append(need_repainted_lines, HandleSelectedBytesOnMouseMove(position));

    RepaintLines(need_repainted_lines);
}


std::vector<std::size_t> BinaryColumnBody::HandleMouseOverBytesOnMouseMove(
    const zaf::Point& mouse_position) {

    std::vector<std::size_t> need_repainted_lines;
    if (mouse_over_byte_index_) {
        need_repainted_lines.push_back(mouse_over_byte_index_->Line());
    }

    bool is_in_character_index{};
    mouse_over_byte_index_ = FindByteIndex(mouse_position, true, is_in_character_index);
    if (mouse_over_byte_index_) {
        need_repainted_lines.push_back(mouse_over_byte_index_->Line());
    }

    return need_repainted_lines;
}


std::vector<std::size_t> BinaryColumnBody::HandleSelectedBytesOnMouseMove(
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


void BinaryColumnBody::OnMouseDown(const zaf::MouseMoveInfo& event_info) {

    __super::OnMouseDown(event_info);

    HandleMouseDown(event_info.PositionAtSender(), event_info.Message());
    event_info.MarkAsHandled();
}


void BinaryColumnBody::HandleMouseDown(
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


void BinaryColumnBody::OnMouseUp(const zaf::MouseUpInfo& event_info) {

    __super::OnMouseUp(event_info);

    HandleMouseUp(event_info.PositionAtSender());
    event_info.MarkAsHandled();
}


void BinaryColumnBody::HandleMouseUp(const zaf::Point& position) {

    if (IsCapturingMouse()) {
        ReleaseCapture();
    }
}


std::vector<std::size_t> BinaryColumnBody::GetSelectedLines() const {

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


void BinaryColumnBody::RepaintLines(const std::vector<std::size_t>& lines) {

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


std::optional<ByteIndex> BinaryColumnBody::FindByteIndex(
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


std::optional<std::size_t> BinaryColumnBody::FindByteIndexInLine(
    float x,
    bool adjust_to_nearest_index,
    bool& is_in_character_area) {

    if (x < ByteHexAreaX()) {
        if (!adjust_to_nearest_index) {
            return std::nullopt;
        }
        return 0;
    }

    if (x < ByteHexFirstPartEndX()) {
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

    //Adjust to the last byte in character area.
    is_in_character_area = true;
    return BytesPerLine - 1;
}


std::optional<std::size_t> BinaryColumnBody::FindByteLine(float y, bool adjust_to_nearest_index) {

    auto adjusted_y = y;

    if (adjust_to_nearest_index) {

        if (adjusted_y < 0) {
            adjusted_y = 0;
        }
    }

    return static_cast<std::size_t>(adjusted_y / LineHeight);
}

}